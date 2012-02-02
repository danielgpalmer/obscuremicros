syscalls.o: syscalls.c syscalls.h
	$(CC) $(CFLAGS) syscalls.c

interrupts.o: interrupts.c
	$(CC) $(CFLAGS) interrupts.c

exceptions.o: exceptions.c
	$(CC) $(CFLAGS) exceptions.c