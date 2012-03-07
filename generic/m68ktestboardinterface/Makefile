CFLAGS=-std=gnu99

all: testharness

m68kinterface.o: m68kinterface.c m68kinterface.h stubs.h
	$(CC) $(CFLAGS) -c m68kinterface.c -o m68kinterface.o

testharness: testharness.c m68kinterface.o
	$(CC) $(CLAGS) testharness.c -o testharness m68kinterface.o
