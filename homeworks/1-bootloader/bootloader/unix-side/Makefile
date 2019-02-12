CFLAGS = -Wall -Og -g -ggdb -I../shared-code
CC = gcc
SRC = my-install.c fatal.c simple-boot.c support.c trace.c tty.c
OBJS = $(SRC:.c=.o)

all : my-install 

my-install:  $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o my-install -lm -g -ggdb

clean :
	rm -f *.o *~ Makefile.bak my-install

depend:
	makedepend *.[ch] 
