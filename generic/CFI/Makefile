all: testharness

cfi.o: cfi.c cfi.h cfistubs.h
	$(CC) -o cfi.o -c cfi.c

testharness: testharness.c cfi.o cfi.h cfistubs.h
	$(CC) -o testharness testharness.c cfi.o 
