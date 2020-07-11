So now the biggest problem we now encounter is how to install and make postgreSQL

 git clone http://git.postgresql.org/git/postgresql.git
 
 cd postgresql
 
 ./configure
 
 make
 
 All of PostgreSQL successfully made. Ready to install.
 
 make world
 
 PostgreSQL, contrib, and documentation successfully made. Ready to install.
 
 make check
 
 make install
 
 make install-docs
 
 make install-world
 
 LD_LIBRARY_PATH=/usr/local/pgsql/lib
export LD_LIBRARY_PATH

/sbin/ldconfig /usr/local/pgsql/lib


PATH=/usr/local/pgsql/bin:$PATH
export PATH

set path = ( /usr/local/pgsql/bin $path )

MANPATH=/usr/local/pgsql/share/man:$MANPATH
export MANPATH

