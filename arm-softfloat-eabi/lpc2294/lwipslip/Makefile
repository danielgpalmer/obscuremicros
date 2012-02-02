include ./common.mk

all: ram

clean:
	-rm crt.lst main.lst crt.o uart.o printf.o sio.o syscalls.o main.o main.out main.hex main.map main.dmp main.bin ping.0

rom: main.out
	@ echo "...copying rom version"
	$(CP) $(CPFLAGS) main.out main.bin
	$(OD) $(ODFLAGS) main.out > main.dmp

ram: main-ram.out
	@ echo "...copying ram version"
	$(CP) $(CPFLAGS) main-ram.out main-ram.bin
	$(OD) $(ODFLAGS) main-ram.out > main-ram.dmp

main-ram.out: crt-ram.o uart.o main.o syscalls.o demo2214-extram.cmd printf.o sio.o net.o echo.o exceptions.o interrupts.o ping.o
	@ echo "..linking ram version"
	##$(LD) $(LFLAGS) -o main.out  crt.o uart.o  main.o
	$(CC) $(LFLAGSRAM) -o main-ram.out crt.o syscalls.o uart.o  main.o printf.o sio.o net.o echo.o exceptions.o interrupts.o ping.o -llwip 

main.out: crt.o uart.o main.o syscalls.o demo2214.cmd printf.o
	@ echo "..linking rom version"
	##$(LD) $(LFLAGS) -o main.out  crt.o uart.o  main.o
	$(CC) $(LFLAGS) -o main.out -llwip crt.o syscalls.o uart.o  main.o printf.o sio.o -llwip -llwip

crt-ram.o: crt-ram.s
	$(AS) $(AFLAGS) crt-ram.s > crt-ram.ls

crt.o: crt.s
	@ echo ".assembling"
	$(AS) $(AFLAGS) crt.s > crt.lst

uart.o: uart.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) uart.c
	

sio.o: sio.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) sio.c

net.o: net.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) net.c

echo.o: echo.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) echo.c

exceptions.o: exceptions.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) exceptions.c

interrupts.o: interrupts.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) interrupts.c

ping.o: ping.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) ping.c
	
main.o: main.c
	@ echo ".compiling"
	$(CC) $(CFLAGS) main.c



syscalls.o: syscalls.c syscalls.h
	$(CC) $(CFLAGS) syscalls.c

