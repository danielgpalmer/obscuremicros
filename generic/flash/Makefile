ifndef OUTPUTDIR
	OUTPUTDIR = ./
endif

CFLAGS = -std=gnu99 -Wall -ggdb

all: testharness libflash.a

$(OUTPUTDIR)/common.o: common.c common.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/cfi.o: cfi.c cfi.h flashstubs.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/jedec.o: jedec.c flashstubs.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/intel.o: intel.c flashstubs.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/atmel.o: atmel.c flashstubs.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/libflash.a: $(OUTPUTDIR)/jedec.o $(OUTPUTDIR)/cfi.o $(OUTPUTDIR)/intel.o $(OUTPUTDIR)/atmel.o $(OUTPUTDIR)/common.o
	$(AR) cr $@ $^	

$(OUTPUTDIR)/testharness: testharness.c cfi.o jedec.o cfi.h flashstubs.h
	$(CC) $(CFLAGS) -o $@ $< cfi.o jedec.o

.PHONY: clean

clean:
	rm -f $(OUTPUTDIR)/libflash.a $(OUTPUTDIR)/cfi.o $(OUTPUTDIR)/jedec.o $(OUTPUTDIR)/intel.o $(OUTPUTDIR)/atmel.o $(OUTPUTDIR)/common.o
