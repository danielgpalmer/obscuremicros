CFLAGS = -std=gnu99

all: testharness

cfi.o: cfi.c cfi.h flashstubs.h
	$(CC) $(CFLAGS) -o cfi.o -c cfi.c

jedec.o: jedec.c flashstubs.h
	$(CC) $(CFLAGS) -o jedec.o -c jedec.c

testharness: testharness.c cfi.o jedec.o cfi.h flashstubs.h
	$(CC) $(CFLAGS) -o testharness testharness.c cfi.o jedec.o
