NAME   = demo2294_blink_flash

TCPREFIX = ../../../toolchains/inst/arm-softfloat-eabi/bin
CC      = $(TCPREFIX)/arm-softfloat-eabi-gcc
LD      = $(TCPREFIX)/arm-softfloat-eabi-ld -v
AR      = $(TCPREFIX)/arm-softfloat-eabi-ar
AS      = $(TCPREFIX)/arm-softfloat-eabi-as
CP      = $(TCPREFIX)/arm-softfloat-eabi-objcopy
OD	= $(TCPREFIX)/arm-softfloat-eabi-objdump

CFLAGS  = -I./ -c -fno-common -O0 -ggdb --std=gnu99
AFLAGS  = -mapcs-32
LFLAGS  =  -nostartfiles  -Wl,-Map=output.map
CPFLAGS = -O binary
ODFLAGS	= -x --syms


all: main.bin main-extram.bin

clean:
	rm -f *.o *.bin *.dmp *.elf

main.bin: main.elf
	$(CP) $(CPFLAGS) main.elf main.bin
	$(OD) $(ODFLAGS) main.elf > main.dmp

main-ram.bin: main-ram.elf
	$(CP) $(CPFLAGS) main-ram.elf main-ram.bin
	$(OD) $(ODFLAGS) main-ram.elf > main-ram.dmp

main-extram.bin: main-extram.elf
	$(CP) $(CPFLAGS) main-extram.elf main-extram.bin
	$(OD) $(ODFLAGS) main-extram.elf > main-extram.dmp

main.elf: crt.o uart.o main.o syscalls.o interrupts.o exceptions.o demo2214.cmd 
	$(CC) -Tdemo2214.cmd $(LFLAGS) -o main.elf crt.o interrupts.o exceptions.o syscalls.o uart.o  main.o

main-ram.elf: crt-ram.o uart.o main.o syscalls.o interrupts.o exceptions.o demo2214-ram.cmd
	$(CC) -Tdemo2214-ram.cmd $(LFLAGS) -o main-ram.elf crt-ram.o interrupts.o exceptions.o syscalls.o uart.o  main.o

main-extram.elf: crt-ram.o uart.o main.o syscalls.o interrupts.o exceptions.o demo2214-extram.cmd
	$(CC) -Tdemo2214-extram.cmd $(LFLAGS) -o main-extram.elf crt-ram.o interrupts.o exceptions.o syscalls.o uart.o  main.o

crt-ram.o: crt-ram.s
	$(AS) $(AFLAGS) crt-ram.s -o crt-ram.o

crt.o: crt.s
	$(AS) $(AFLAGS) crt.s -o crt.o

uart.o: uart.c
	$(CC) $(CFLAGS) uart.c

interrupts.o: interrupts.c
	$(CC) $(CFLAGS) interrupts.c

exceptions.o: exceptions.c
	$(CC) $(CFLAGS) exceptions.c

main.o: main.c
	$(CC) $(CFLAGS) main.c

syscalls.o: syscalls.c syscalls.h
	$(CC) $(CFLAGS) syscalls.c
