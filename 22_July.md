**How to start postgre**

    pg_ctl -D /usr/local/pgsql/data -l logfile start

![enter image description here](https://i.ibb.co/prGnQY9/Annotation-2020-07-22-132803.png)
bash-4.2$

     pg_ctl -D /usr/local/pgsql/data -l logfile start


Shutdown postgres

$ **``kill -INT `head -1 /usr/local/pgsql/data/postmaster.pid` ``**


Important 

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <liburing.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

gcc dee.c -luring -std=gnu99
