ifndef OUTPUTDIR
	OUTPUTDIR = ./
endif

CFLAGS = -std=gnu99

all: testharness libflash.a

$(OUTPUTDIR)/cfi.o: cfi.c cfi.h flashstubs.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/jedec.o: jedec.c flashstubs.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/intel.o: intel.c flashstubs.h
	$(CC) $(CFLAGS) -o $@ -c $<

$(OUTPUTDIR)/libflash.a: $(OUTPUTDIR)/jedec.o $(OUTPUTDIR)/cfi.o $(OUTPUTDIR)/intel.o
	$(AR) cr $@ $^	

$(OUTPUTDIR)/testharness: testharness.c cfi.o jedec.o cfi.h flashstubs.h
	$(CC) $(CFLAGS) -o $@ $< cfi.o jedec.o

.PHONY: clean

clean:
	rm -f $(OUTPUTDIR)/libflash.a $(OUTPUTDIR)/cfi.o $(OUTPUTDIR)/jedec.o $(OUTPUTDIR)/intel.o
