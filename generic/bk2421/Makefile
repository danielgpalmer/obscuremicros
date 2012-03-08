all: bk2421spidevtest

bk2421.o: bk2421.c bk2421.h bk2421io.h
	$(CC) -ggdb -c bk2421.c -o bk2421.o
bk2421spidevtest: bk2421spidevtest.c bk2421.h bk2421io.h bk2421.o
	$(CC) -ggdb bk2421spidevtest.c bk2421.o -o bk2421spidevtest.o

.PHONY: clean

clean:
	rm -f *.o bk2421spidevtest
