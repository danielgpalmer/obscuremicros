include ./common.mk

LWIPCFLAGS = -I ./lwip/src/include/ -I ./lwip/src/include/ipv4/ -I./lwip/src/include/ipv6/
FATFSCFLAGS = -I ./fatfs/src/

all: main-extram.bin flashfs.bin

include ./startup.mk
include ./sys.mk
include ./peripherals.mk

rom: main.elf
	$(CP) $(CPFLAGS) main.out main.bin
	$(OD) $(ODFLAGS) main.out > main.dmp

main-extram.bin: main-extram.elf
	$(CP) $(CPFLAGS) main-extram.elf main-extram.bin

main-extram.elf: ldscript-extram.x crt-ram.o uart.o main.o syscalls.o net.o echo.o exceptions.o interrupts.o core.o liblwip.a libfatfs.a
	$(CC) -Tldscript-extram.x $(LFLAGS) -o main-extram.elf crt-ram.o syscalls.o uart.o  main.o net.o echo.o exceptions.o interrupts.o rtc.o core.o ./liblwip.a ./libfatfs.a

main.elf: crt.o uart.o main.o syscalls.o printf.o
	@ echo "..linking rom version"
	$(CC) $(LFLAGS) -o main.out -llwip crt.o syscalls.o uart.o  main.o printf.o rtc.o ./liblwip.a

#main.elf: crt.o uart.o timer.o main.o syscalls.o interrupts.o exceptions.o adc.o wdt.o rtc.o i2c.o ldscript-intflash.x 
#        $(CC) -Tldscript-intflash.x $(LFLAGS) -o main.elf crt.o interrupts.o exceptions.o syscalls.o uart.o timer.o adc.o wdt.o rtc.o i2c.o main.o

#main-ram.elf: crt-ram.o uart.o timer.o main.o syscalls.o interrupts.o exceptions.o adc.o wdt.o rtc.o i2c.o ldscript-intram.x
#        $(CC) -Tldscript-intram.x $(LFLAGS) -o main-ram.elf crt-ram.o interrupts.o exceptions.o syscalls.o uart.o timer.o adc.o wdt.o rtc.o i2c.o main.o

#main-extram.elf: crt-ram.o uart.o timer.o main.o syscalls.o interrupts.o exceptions.o adc.o wdt.o rtc.o ldscript-extram.x
#        $(CC) -Tldscript-extram.x $(LFLAGS) -o main-extram.elf crt-ram.o interrupts.o exceptions.o syscalls.o uart.o timer.o adc.o wdt.o rtc.o i2c.o main.o

net.o: net.c
	$(CC) $(CFLAGS) $(LWIPCFLAGS) net.c

echo.o: echo.c
	$(CC) $(CFLAGS) $(LWIPCFLAGS) echo.c
	
main.o: main.c
	$(CC) $(CFLAGS) $(LWIPCFLAGS) $(FATFSCFLAGS) main.c

.PHONY: clean liblwip.a libfatfs.a flashfs.bin debug

liblwip.a:
	$(MAKE) -f lwip.mk

libfatfs.a:
	$(MAKE) -f fatfs.mk

clean:	
	rm -f *.o *.elf *.bin

debug: main-extram.bin main-extram.elf
	$(GDB)

flashfs.bin:
	dd if=/dev/zero of=flashfs.bin bs=1M count=4
	/sbin/mkfs.vfat flashfs.bin
	MTOOLSRC=./mtools.conf mcopy flashfs/text.txt m:/
	MTOOLSRC=./mtools.conf mdir m:
