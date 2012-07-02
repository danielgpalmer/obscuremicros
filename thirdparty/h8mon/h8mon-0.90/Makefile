VERSION = $(shell cat VERSION)

CPUNAME	= 3069
LDSCRIPT= mon3069.lds
DBGLDSCRIPT= mon3069-dbg.lds
PROG	= mon

ASFLAGS	= -W
LDFLAGS = -mh -Wl,-Map,$(PROGRAM).map -Wl,-T$(LDSCRIPT) -nostartfiles #-nostdlib
DBGLDFLAGS = -mh -Wl,-Map,$(PROGRAM)-dbg.map -Wl,-T$(DBGLDSCRIPT) -nostartfiles #-nostdlib

#--- Select Program Writer Serial Port (for h8write)
H8WPORT = /dev/ttyS0
#H8WPORT = COM3

#--- Select Compiler
#CROSS	= h8300-hms-
#CROSS	= h8300-coff-
CROSS	= h8300-elf-

AS	= $(CROSS)as
CC	= $(CROSS)gcc
AR	= $(CROSS)ar
RANLIB	= $(CROSS)ranlib
LD	= $(CROSS)gcc
OBJCOPY	= $(CROSS)objcopy
STRIP	= $(CROSS)strip

CFLAGS	= -g -O2 -Wall -mh #-DDEBUG

PROGRAM	= h8$(CPUNAME)$(PROG)-$(VERSION)

DEPEND	= .depend

NET_DIR = net
NET_LIB = $(NET_DIR)/net.a

AOBJS	= section.o start.o vvects.o tasks.o
ASRCS   = section.s start.s vvects.s tasks.s

OBJS    = init3069f.o vect.o sci1.o \
	  lineedit.o shell.o commands.o sformat.o history.o mon.o main.o \
	  conio.o monio.o fifo.o task.o stub.o str.o disasm.o tprintf.o \
	  vtprintf.o initsect.o sci0.o

DBGOBJS = init3069f.o dvect.o sci1.o \
	  lineedit.o shell.o commands.o sformat.o history.o mon.o main.o \
	  conio.o monio.o fifo.o task.o stub.o str.o disasm.o tprintf.o \
	  vtprintf.o initsect.o sci0.o

SRCS    = init3069f.c vect.c sci1.c \
	  lineedit.c shell.c commands.c sformat.c history.c mon.c main.c \
	  conio.c monio.c fifo.c task.c stub.c str.c disasm.c tprintf.c \
	  vtprintf.c initsect.c sci0.c

.SUFFIXES:	.mot .exe .a .o .c .s .h

.c.o:
	$(CC) $(CFLAGS) -c $<

.exe.mot:
	$(OBJCOPY) -Osrec $< $@

all:	$(PROGRAM).mot

$(PROGRAM).mot: $(OBJS) $(AOBJS) $(LDSCRIPT) $(DEPEND) $(NET_LIB)
	echo "const char version[] = \"$(VERSION)\";" > version.c
	echo "const char date[] = __DATE__;" >> version.c
	echo "const char time[] = __TIME__;" >> version.c
	$(CC) $(CFLAGS) -c version.c
	$(LD) $(LDFLAGS) -o $(PROGRAM).exe $(OBJS) $(AOBJS) \
	version.o $(LIBS) $(NET_LIB) -lgcc
	$(OBJCOPY) -Osrec $(PROGRAM).exe $(PROGRAM).mot

debug: $(DBGOBJS) section.o start.o tasks.o $(LDSCRIPT) $(DEPEND) $(NET_LIB)
	echo "const char version[] = \"$(VERSION)-debug\";" > version.c
	echo "const char date[] = __DATE__;" >> version.c
	echo "const char time[] = __TIME__;" >> version.c
	$(CC) $(CFLAGS) -c version.c
	$(LD) $(DBGLDFLAGS) -o $(PROGRAM)-dbg.exe start.o $(DBGOBJS) \
	section.o tasks.o version.o $(LIBS) $(NET_LIB) -lgcc
	$(STRIP) $(PROGRAM)-dbg.exe
	$(OBJCOPY) -Osrec $(PROGRAM)-dbg.exe $(PROGRAM)-dbg.mot

$(NET_LIB): $(NET_DIR)/*.c
	make -C $(NET_DIR)

h8disasm: disasm.c sformat.c str.c
	gcc -o h8disasm -DTEST -g -O disasm.c sformat.c str.c

write:
	h8write -$(CPUNAME) $(PROGRAM) $(H8WPORT)

tag:
TAGS:
	etags $(SRCS) $(ASRCS) *.h

clean:
	make -C $(NET_DIR) clean
	rm -f -r $(OBJS) $(AOBJS) $(PROGRAM) \
	$(DEPEND) *.exe *.mot *.map *.BAK *~ *.bak Tags TAGS \
	version.c version.o \
	h8disasm

$(DEPEND):
	$(CC) -M $(CFLAGS) $(SRCS) $(ASRCS) > $(DEPEND)

-include $(DEPEND)
