include ./common.mk

all: main.bin main-extram.bin

include ./startup.mk
include ./sys.mk
include ./peripherals.mk

OBJS = uart.o timer.o main.o syscalls.o interrupts.o exceptions.o adc.o wdt.o rtc.o core.o i2c.o libflash.a ymodem.o

main.bin: main.elf
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.dmp

main-ram.bin: main-ram.elf
	$(CP) $(CPFLAGS) main-ram.elf main-ram.bin
	$(OD) $(ODFLAGS) main-ram.elf > main-ram.dmp

main-extram.bin: main-extram.elf
	$(CP) $(CPFLAGS) main-extram.elf main-extram.bin
	$(OD) $(ODFLAGS) main-extram.elf > main-extram.dmp

main.elf: crt.o $(OBJS) ldscript-intflash.x
	$(CC) -Tldscript-intflash.x $(LFLAGS) -o $@ crt.o $(OBJS) -lm

main-ram.elf: crt-ram.o $(OBJS) ldscript-intram.x
	$(CC) -Tldscript-intram.x $(LFLAGS) -o $@ crt-ram.o $(OBJS) -lm

main-extram.elf: crt-ram.o $(OBJS) ldscript-extram.x
	$(CC) -Tldscript-extram.x $(LFLAGS) -o $@ crt-ram.o $(OBJS) -lm

main.o: main.c
	$(CC) $(CFLAGS) main.c

ymodem.o: ymodem.c ymodem.h
	$(CC) $(CFLAGS) -DDEBUG ymodem.c

debug: main-extram.bin main-extram.elf
	$(GDB)

.PHONY: clean rtcpreload.h libflash.a

libflash.a: 
	$(MAKE) CC=$(TOP)/$(CC) AR=$(TOP)/$(AR) OUTPUTDIR=$(TOP) -C ./flash $(TOP)"/libflash.a"

clean:
	rm -f *.o *.bin *.dmp *.elf
	$(MAKE) CC=$(TOP)/$(CC) AR=$(TOP)/$(AR) OUTPUTDIR=$(TOP) -C ./flash clean

rtcpreload.h:
	./mkrtcpreload.sh > rtcpreload.h

