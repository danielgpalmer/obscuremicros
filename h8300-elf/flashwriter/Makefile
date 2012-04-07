include ./common.mk

CFLAGS	= -I/opt/h8300-elf/ -mh -g -O2 -Wall -save-temps -std=gnu99 -Wstrict-prototypes

LDSCRIPT= h83069.lds

LDFLAGS = -mh -Wl,-Map,$*.map -Wl,-T$(LDSCRIPT) -nostartfiles -L../../toolchains/inst/h8300-elf/h8300-elf/lib/h8300h/

PROGRAM	= h8skel.mot

OBJS          = start.o main.o monitor.o sys.o vect3069.o ymodem.o itoa.o printf.o

SRCS          = start.s main.c monitor.c sys.c vect3069.c ymodem.c itoa.c printf.c

.SUFFIXES:	.mot .exe .a .o .c .s .h

.c.o:	
	$(CC) $(CFLAGS) -c $<

.elf.mot:
	$(CP) -Osrec $< $@

all: $(PROGRAM)

libflash.a: 
	$(MAKE) CC=$(TOP)/$(CC) AR=$(TOP)/$(AR) OUTPUTDIR=$(TOP) EXTRACFLAGS="-mh -DFLASH_DONTWANT_INTEL -DFLASH_DONTWANT_CFI -DFLASH_DONTWANT_JEDEC -DFLASH_DONTUSE_MALLOC" -C ./flash $(TOP)"/libflash.a"

$(PROGRAM): $(OBJS) $(START) $(LDSCRIPT) $(DEPEND) libflash.a
	$(CC) $(LDFLAGS) -o $*.elf $(START) $(OBJS) libflash.a
	$(CP) -Osrec $*.elf $*.mot

clean:
	rm -f -r $(OBJS) $(PROGRAM) $(DEPEND) *.mot *.map *.elf *~
	$(MAKE) OUTPUTDIR=$(TOP) -C./flash clean
