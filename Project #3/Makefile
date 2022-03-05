CFLAGS= -g
LDFLAGS= -lpthread -lrt
LDFLAGS1= -lpthread
CC=g++

all: boundedBuffer condvar1

# To make an executable
boundedBuffer: boundedBuffer.o 
	$(CC) -o boundedBuffer boundedBuffer.o $(LDFLAGS)

condvar1: condvar1.o 
	$(CC) -o condvar1 condvar1.o $(LDFLAGS1)
 
# To make an object from source
.c.o:
	$(CC) $(CFLAGS) -c $*.c

# clean out the dross
clean:
	-rm boundedBuffer condvar1 *.o

