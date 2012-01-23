CROSS   = h8300-elf-

CC	= $(CROSS)gcc
AS	= $(CROSS)as
LD	= $(CROSS)gcc
OBJCOPY	= $(CROSS)objcopy

CFLAGS	= -I/opt/h8300-elf/ -mh -g -O2 -Wall -save-temps

LDSCRIPT= h83069.lds

LDFLAGS = -mh -Wl,-Map,$*.map -Wl,-T$(LDSCRIPT) -nostartfiles -L/opt/toolchains/h8300-elf/h8300-elf/lib/h8300h/

PROGRAM	= h8skel.mot

OBJS          = start.o main.o monitor.o sys.o vect3069.o

SRCS          = start.s main.c monitor.c sys.c vect3069.c

.SUFFIXES:	.mot .exe .a .o .c .s .h

.c.o:	
	$(CC) $(CFLAGS) -c $<

.elf.mot:
	$(OBJCOPY) -Osrec $< $@

all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(START) $(LDSCRIPT) $(DEPEND)
	$(LD) $(LDFLAGS) -o $*.elf $(START) $(OBJS) -lgcc
	$(OBJCOPY) -Osrec $*.elf $*.mot

clean:
	rm -f -r $(OBJS) $(PROGRAM) $(DEPEND) *.mot *.map *.elf *~
