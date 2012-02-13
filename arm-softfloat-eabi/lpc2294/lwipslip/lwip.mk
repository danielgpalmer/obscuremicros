include ./common.mk

LWIPSRC = ./lwip/src/
LWIPINC = -I./lwip/src/include/ -I./lwip/src/include/ipv4 -I./lwip/src/include/ipv6 -I./lwip/src/include/netif/


SOURCES = \
api/err.c \
core/init.c \
core/mem.c \
core/memp.c \
core/netif.c \
core/pbuf.c \
core/raw.c \
core/sys.c \
core/stats.c \
core/def.c \
core/dhcp.c \
core/dns.c \
core/udp.c \
core/tcp.c \
core/tcp_in.c \
core/tcp_out.c \
core/timers.c \
core/inet_chksum.c \
core/ipv4/icmp.c \
core/ipv4/ip4.c \
core/ipv4/ip4_addr.c \
core/ipv4/ip_frag.c \
netif/etharp.c \
netif/slipif.c

REALSOURCES=$(addprefix $(LWIPSRC),$(SOURCES))

OBJECTS=$(notdir $(REALSOURCES:.c=.o))

OUTPUT=liblwip.a

all: $(REALSOURCES) $(OUTPUT) 
	
$(OUTPUT): $(OBJECTS) sys_now.o 
	$(AR) rcs $(OUTPUT) $(OBJECTS) sys_now.o

.PHONY: all depend clean

sys_now.o: sys_now.c
	$(CC) $(CFLAGS) $(LWIPINC) sys_now.c

%.o:
	$(CC) $(CFLAGS) $(LWIPINC) $(<:.o=.c)

clean:
	rm -f *.o $(LWIPLIB) .depend*

depend dep: .depend

include .depend

.depend: $(REALSOURCES)
	$(CC) $(CFLAGS) $(LWIPINC) -MM $^ > .depend || rm -f .depend
