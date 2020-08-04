#include "postgres.h"

#include "access/xlog.h"
#include "access/xlog_internal.h"
#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "pgstat.h"
#include "postmaster/bgwriter.h"
#include "postmaster/interrupt.h"
#include "storage/buf_internals.h"
#include "storage/bufmgr.h"
#include "storage/condition_variable.h"
#include "storage/fd.h"
#include "storage/ipc.h"
#include "storage/lwlock.h"
#include "storage/proc.h"
#include "storage/procsignal.h"
#include "storage/shmem.h"
#include "storage/smgr.h"
#include "storage/spin.h"
#include "storage/standby.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/resowner.h"
#include "utils/timestamp.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <liburing.h>
#include <stdlib.h>

int BgWriterDelay = 200;

#define HIBERNATE_FACTOR 50

#define LOG_SNAPSHOT_INTERVAL_MS 15000

static TimestampTz last_snapshot_ts;
static XLogRecPtr last_snapshot_lsn = InvalidXLogRecPtr;

//Begin from here

void BackgroundWriterMain(void)
{

	struct io_uring
	{
		struct io_uring_sq sq;
		struct io_uring_cq cq;
		unsigned flags;
		int ring_fd;
	};

	int setup_io_uring(int efd)
	{
		int ret = io_uring_queue_init(8, &ring, 0);
		if (ret)
		{
			fprintf(stderr, "Unable to setup io_uring: %s\n", strerror(-ret));
			return 1;
		}
		io_uring_register_eventfd(&ring, efd);
		return 0;
	}

	sigjmp_buf local_sigjmp_buf;
	MemoryContext bgwriter_context;
	bool prev_hibernate;
	WritebackContext wb_context;

	pqsignal(SIGHUP, SignalHandlerForConfigReload);
	pqsignal(SIGINT, SIG_IGN);
	pqsignal(SIGTERM, SignalHandlerForShutdownRequest);
	pqsignal(SIGQUIT, SignalHandlerForCrashExit);
	pqsignal(SIGALRM, SIG_IGN);
	pqsignal(SIGPIPE, SIG_IGN);
	pqsignal(SIGUSR1, procsignal_sigusr1_handler);
	pqsignal(SIGUSR2, SIG_IGN);

	pqsignal(SIGCHLD, SIG_DFL);

	sigdelset(&BlockSig, SIGQUIT);

	last_snapshot_ts = GetCurrentTimestamp();

	bgwriter_context = AllocSetContextCreate(TopMemoryContext,
											 "Background Writer",
											 ALLOCSET_DEFAULT_SIZES);
	MemoryContextSwitchTo(bgwriter_context);

	WritebackContextInit(&wb_context, &bgwriter_flush_after);

	if (sigsetjmp(local_sigjmp_buf, 1) != 0)
	{

		error_context_stack = NULL;

		HOLD_INTERRUPTS();

		EmitErrorReport();

		LWLockReleaseAll();
		ConditionVariableCancelSleep();
		AbortBufferIO();
		UnlockBuffers();
		ReleaseAuxProcessResources(false);
		AtEOXact_Buffers(false);
		AtEOXact_SMgr();
		AtEOXact_Files(false);
		AtEOXact_HashTables(false);

		MemoryContextSwitchTo(bgwriter_context);
		FlushErrorState();

		MemoryContextResetAndDeleteChildren(bgwriter_context);

		WritebackContextInit(&wb_context, &bgwriter_flush_after);

		RESUME_INTERRUPTS();

		pg_usleep(1000000L);

		smgrcloseall();

		pgstat_report_wait_end();
	}

	PG_exception_stack = &local_sigjmp_buf;

	PG_SETMASK(&UnBlockSig);

	prev_hibernate = false;

	for (;;)
	{
		bool can_hibernate;
		int rc;

		ResetLatch(MyLatch);

		HandleMainLoopInterrupts();

		can_hibernate = BgBufferSync(&wb_context);

		pgstat_send_bgwriter();

		if (FirstCallSinceLastCheckpoint())
		{

			smgrcloseall();
		}

		if (XLogStandbyInfoActive() && !RecoveryInProgress())
		{
			TimestampTz timeout = 0;
			TimestampTz now = GetCurrentTimestamp();

			timeout = TimestampTzPlusMilliseconds(last_snapshot_ts,
												  LOG_SNAPSHOT_INTERVAL_MS);

			if (now >= timeout &&
				last_snapshot_lsn <= GetLastImportantRecPtr())
			{
				last_snapshot_lsn = LogStandbySnapshot();
				last_snapshot_ts = now;
			}
		}

		rc = WaitLatch(MyLatch,
					   WL_LATCH_SET | WL_TIMEOUT | WL_EXIT_ON_PM_DEATH,
					   BgWriterDelay, WAIT_EVENT_BGWRITER_MAIN);

		if (rc == WL_TIMEOUT && can_hibernate && prev_hibernate)
		{

			StrategyNotifyBgWriter(MyProc->pgprocno);

			(void)WaitLatch(MyLatch,
							WL_LATCH_SET | WL_TIMEOUT | WL_EXIT_ON_PM_DEATH,
							BgWriterDelay * HIBERNATE_FACTOR,
							WAIT_EVENT_BGWRITER_HIBERNATE);

			StrategyNotifyBgWriter(-1);
		}

		prev_hibernate = can_hibernate;
	}

	pthread_t t;
	int efd;

	/* Create an eventfd instance */
	efd = eventfd(0, 0);
	if (efd < 0)
		error_exit("eventfd");

	/* Create the listener thread */
	pthread_create(&t, NULL, listener_thread, (void *)efd);

	sleep(2);

	/* Setup io_uring instance and register the eventfd */
	setup_io_uring(efd);

	/* Initiate a read with io_uring */
	read_file_with_io_uring();

	/* Wait for th listener thread to complete */
	pthread_join(t, NULL);

	/* All done. Clean up and exit. */
	io_uring_queue_exit(&ring);
	return EXIT_SUCCESS;
}
