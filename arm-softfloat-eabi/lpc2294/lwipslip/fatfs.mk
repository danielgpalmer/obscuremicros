include ./common.mk

all: libfatfs.a

libfatfs.a: ff.o diskio.o
	$(AR) rcs libfatfs.a ff.o diskio.o

ff.o: fatfs/src/ff.c
	$(CC) $(CFLAGS) -c fatfs/src/ff.c -o ff.o

diskio.o: diskio.c
	$(CC) $(CFLAGS) $(FATFSCFLAGS) -c diskio.c -o diskio.o

