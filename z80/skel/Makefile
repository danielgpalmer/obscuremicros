CC=sdcc

all:main.bin

ctc.o: ctc.c
	$(CC) -mz80 -c ctc.c

sio.o: sio.c
	$(CC) -mz80 -c sio.c

pio.o: pio.c
	$(CC) -mz80 -c pio.c

cgc.o: cgc.c
	$(CC) -mz80 -c cgc.c

wdt.o: wdt.c
	$(CC) -mz80 -c wdt.c
	
main.o:
	$(CC) -mz80 -c main.

main.ihx: main.o ctc.o sio.o pio.o cgc.o wdt.o
	$(CC) -mz80 --data-loc 0x8000 $^

main.bin: main.ihx
	objcopy -Iihex -Obinary main.ihx main.bin

.PHONY: clean

clean:
	rm -f *.o
