CC=sdcc

all: main.bin

ctc.o: ctc.c ctc.h
	$(CC) -mz80 -c ctc.c

sio.o: sio.c sio.h
	$(CC) -mz80 -c sio.c

pio.o: pio.c pio.h
	$(CC) -mz80 -c pio.c

wdt.o: wdt.c wdt.h
	$(CC) -mz80 -c wdt.c
	
main.o:
	$(CC) -mz80 -c main.c

main.ihx: main.o ctc.o sio.o pio.o wdt.o
	$(CC) -mz80 --data-loc 0x8000 $^

main.bin: main.ihx
	objcopy -Iihex -Obinary main.ihx main.bin

.PHONY: clean

clean:
	rm -f *.o *.sym *.lst *.asm *.bin *.ihx *.lnk *.map
