crt-ram.o: crt-ram.s
	$(AS) $(AFLAGS) crt-ram.s -o crt-ram.o

crt.o: crt.s
	$(AS) $(AFLAGS) crt.s -o crt.o

