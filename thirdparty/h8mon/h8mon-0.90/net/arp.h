/*
 * ARP
 *
 * 2011.01.15	Takashi SHUDO
 */

#ifndef _H_arp
#define _H_arp

#include "ip.h"
#include "ethernet.h"

#define ARP_MES_LEN	28

struct ARP_HEADER {
	unsigned short	hardware;
#define ARP_HARDWARE_ETHER 1
	
	unsigned short	protocol;
	unsigned char	macaddr_size;
	unsigned char	ipaddr_size;
	unsigned short	operation;
#define ARP_OPERATION_REQUEST 1
#define ARP_OPERATION_REPLY   2
	
	struct MAC_ADDR	sender_macaddr;
	struct IP_ADDR	sender_ipaddr;
	struct MAC_ADDR	target_macaddr;
	struct IP_ADDR	target_ipaddr;
} __attribute__((packed));

#define MAX_ARP_CACHE	4

struct ARP_TABLE {
	int use;
	struct IP_ADDR ipaddr;
	struct MAC_ADDR	macaddr;
} ARP_TABEL;

int init_arp(void);
int search_ipaddr_mac(struct IP_ADDR *ipaddr, struct MAC_ADDR **macaddr);
int cache_arp(struct IP_ADDR *ipaddr, struct MAC_ADDR *macaddr);
int send_arp_request(struct IP_ADDR *ipaddr);
int proc_arp(unsigned char *buf, int len);

#endif // _H_arp
