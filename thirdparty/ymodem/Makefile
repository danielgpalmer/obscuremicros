LFLAGS=`pkg-config --libs argtable2`
CFLAGS=`pkg-config --cflags argtable2`


all: testharness

ymodem.o: ymodem.c ymodem.h
	$(CC) -c $< -o $@

testharness.o: testharness.c
	$(CC) $(CFLAGS) -c $< -o $@

testharness: testharness.o ymodem.o
	$(CC) $(LFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f testharness *.o
