CROSS   = h8300-elf-

CC	= $(CROSS)gcc
AS	= $(CROSS)as
LD	= $(CROSS)gcc
OBJCOPY	= $(CROSS)objcopy

CFLAGS	= -I/opt/toolchains/h8300-elf/ -mh -g -O2 -Wall -save-temps

LDSCRIPT= h83069.lds

LDFLAGS = -mh -Wl,-Map,$*.map -Wl,-T$(LDSCRIPT) -nostartfiles -L/opt/toolchains/h8300-elf/h8300-elf/lib/h8300h/

PROGRAM	= tetris.mot

OBJS          = start.o main.o monitor.o sys.o vect3069.o display.o nvram.o input.o audio.o debugleds.o detection.o

SRCS          = start.s main.c monitor.c sys.c vect3069.c display.c nvram.c input.c audio.c debugleds.c detection.c

.SUFFIXES:	.mot .exe .a .o .c .s .h

.c.o:	
	$(CC) $(CFLAGS) -c $<

.elf.mot:
	$(OBJCOPY) -Osrec $< $@

all: $(PROGRAM)

$(PROGRAM): $(OBJS) $(START) $(LDSCRIPT) $(DEPEND)
	$(LD) $(LDFLAGS) -o $*.elf $(START) $(OBJS) -lm -lgcc
	$(OBJCOPY) -Osrec $*.elf $*.mot

clean:
	rm -f -r $(OBJS) $(PROGRAM) $(DEPEND) *.mot *.map *.elf *~
