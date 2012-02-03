uart.o: uart.c
	$(CC) $(CFLAGS) uart.c

timer.o: timer.c
	$(CC) $(CFLAGS) timer.c

wdt.o: wdt.c wdt.h
	$(CC) $(CFLAGS) wdt.c

audio.o: audio.c audio.h
	$(CC) $(CFLAGS) audio.c

rtc.o: rtc.c rtc.h rtcpreload.h
	$(CC) $(CFLAGS) rtc.c

i2c.o: i2c.c i2c.h
	$(CC) $(CFLAGS) i2c.c
