include ./common.mk

all: main.bin main-extram.bin

include ./startup.mk

main.bin: main.elf
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.dmp

main-ram.bin: main-ram.elf
	$(CP) $(CPFLAGS) main-ram.elf main-ram.bin
	$(OD) $(ODFLAGS) main-ram.elf > main-ram.dmp

main-extram.bin: main-extram.elf
	$(CP) $(CPFLAGS) main-extram.elf main-extram.bin
	$(OD) $(ODFLAGS) main-extram.elf > main-extram.dmp

main.elf: crt.o uart.o timer.o main.o syscalls.o interrupts.o exceptions.o adc.o wdt.o rtc.o i2c.o ldscript-intflash.x 
	$(CC) -Tldscript-intflash.x $(LFLAGS) -o main.elf crt.o interrupts.o exceptions.o syscalls.o uart.o timer.o adc.o wdt.o rtc.o i2c.o main.o

main-ram.elf: crt-ram.o uart.o timer.o main.o syscalls.o interrupts.o exceptions.o adc.o wdt.o rtc.o i2c.o ldscript-intram.x
	$(CC) -Tldscript-intram.x $(LFLAGS) -o main-ram.elf crt-ram.o interrupts.o exceptions.o syscalls.o uart.o timer.o adc.o wdt.o rtc.o i2c.o main.o

main-extram.elf: crt-ram.o uart.o timer.o main.o syscalls.o interrupts.o exceptions.o adc.o wdt.o rtc.o ldscript-extram.x
	$(CC) -Tldscript-extram.x $(LFLAGS) -o main-extram.elf crt-ram.o interrupts.o exceptions.o syscalls.o uart.o timer.o adc.o wdt.o rtc.o i2c.o main.o

uart.o: uart.c
	$(CC) $(CFLAGS) uart.c

timer.o: timer.c
	$(CC) $(CFLAGS) timer.c

interrupts.o: interrupts.c
	$(CC) $(CFLAGS) interrupts.c

exceptions.o: exceptions.c
	$(CC) $(CFLAGS) exceptions.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

syscalls.o: syscalls.c syscalls.h
	$(CC) $(CFLAGS) syscalls.c

wdt.o: wdt.c wdt.h
	$(CC) $(CFLAGS) wdt.c

audio.o: audio.c audio.h
	$(CC) $(CFLAGS) audio.c

rtc.o: rtc.c rtc.h rtcpreload.h
	$(CC) $(CFLAGS) rtc.c

i2c.o: i2c.c i2c.h
	$(CC) $(CFLAGS) i2c.c

debug: main-extram.bin main-extram.elf
	$(GDB)


.PHONY: clean rtcpreload.h

clean:
	rm -f *.o *.bin *.dmp *.elf

rtcpreload.h:
	./mkrtcpreload.sh > rtcpreload.h

