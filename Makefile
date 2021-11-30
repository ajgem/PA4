CC=gcc
CFLAGS=-g -Wall -Werror -fsanitize=address

all:
$(CC) $(CFLAGS) -c -o mymalloc.o mymalloc.c
$(CC) $(CFLAGS) -o memperf memperf.c mymalloc.o

clean:
rm -f *.o
rm -f memperf
