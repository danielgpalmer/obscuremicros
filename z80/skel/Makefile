CC=sdcc

all:main.bin

main.o:
	$(CC) -mz80 -c main.c

main.ihx: main.o
	$(CC) -mz80 --data-loc 0x8000 main.o

main.bin: main.ihx
	objcopy -Iihex -Obinary main.ihx main.bin

.PHONY: clean

clean:
	rm -f *.o
