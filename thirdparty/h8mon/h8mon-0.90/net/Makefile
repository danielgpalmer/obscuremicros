CROSS	= h8300-elf-

AS	= $(CROSS)as
CC	= $(CROSS)gcc
AR	= $(CROSS)ar
RANLIB	= $(CROSS)ranlib
LD	= $(CROSS)gcc
OBJCOPY	= $(CROSS)objcopy
CFLAGS	= -g -O2 -Wall -mh -I..

LIB	= net.a

DEPEND	= .depend

OBJS	= rtl8019as.o \
	  nettimer.o \
	  ethernet.o \
	  ip.o \
	  arp.o \
	  icmp.o \
	  udp.o \
	  net.o \
	  netstr.o \
	  tftpget.o \
	  elf.o

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(LIB)

$(LIB): $(OBJS)
	$(AR) rc $(LIB) $(OBJS)
	$(RANLIB) $(LIB)

clean:
	rm -f -r $(OBJS) $(LIB) $(DEPEND) Tags TAGS *~

$(DEPEND): $(OBJS:.o=.c)
	-@ rm -f $(DEPEND)
	-@ for i in $^; do\
		$(CC) -M $(CFLAGS) $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.c//g" >> $(DEPEND);\
	done

-include $(DEPEND)
