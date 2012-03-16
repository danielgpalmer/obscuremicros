CFLAGS = -std=gnu99

all: testharness

cfi.o: cfi.c cfi.h cfistubs.h
	$(CC) $(CFLAGS) -o cfi.o -c cfi.c

testharness: testharness.c cfi.o cfi.h cfistubs.h
	$(CC) $(CFLAGS) -o testharness testharness.c cfi.o 
