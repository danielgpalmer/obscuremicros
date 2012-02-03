core.o: core.c core.h
	$(CC) $(CFLAGS) core.c

syscalls.o: syscalls.c syscalls.h rtc.o
	$(CC) $(CFLAGS) syscalls.c

interrupts.o: interrupts.c
	$(CC) $(CFLAGS) interrupts.c

exceptions.o: exceptions.c
	$(CC) $(CFLAGS) exceptions.c
