
    pg_ctl -D /usr/local/pgsql/data -l logfile start

![enter image description here](https://i.ibb.co/prGnQY9/Annotation-2020-07-22-132803.png)
bash-4.2$

     pg_ctl -D /usr/local/pgsql/data -l logfile start


Shutdown postgres

$ **``kill -INT `head -1 /usr/local/pgsql/data/postmaster.pid` ``**


**Important** 

    #include <stdio.h>
    #include <string.h>
    #include <sys/stat.h>
    #include <sys/ioctl.h>
    #include <liburing.h>
    #include <stdlib.h>
    #include <sys/stat.h>
    #include <fcntl.h>

**compile**

    gcc dee.c -luring -std=gnu99 -lpthread

**How to compile bgwriter file**

     gcc bgwriter.c  -I /usr/local/pgsql/include/server/

**to run pg ready set environment variables**

    https://www.postgresql.org/docs/9.1/install-post.html
