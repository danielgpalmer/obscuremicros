/*
 * Ethernet Header
 *
 * 2011.01.15 Takashi SHUDO
 */

#ifndef _H_ethernet
#define _H_ethernet

#include "../str.h"

#define ETHER_P_IP	0x0800		// IP
#define ETHER_P_ARP	0x0806		// ARP

#define MAC_LEN	6
#define ETH_HDR_LEN	14
#define ETH_FRM_LEN	1536

struct MAC_ADDR {
	unsigned char addr[MAC_LEN];
} __attribute__((packed));

struct ETHER_HEADER {
	struct MAC_ADDR	dst_mac;
	struct MAC_ADDR	src_mac;
	unsigned short protocol;
} __attribute__((packed));

#define copy_mac(dst,src)	memorycopy((dst),(src),MAC_LEN)
#define comp_mac(dst,src)	strncomp((char *)(dst),(char *)(src),MAC_LEN)


int init_eth(void);
int send_eth(unsigned char *buf, int len, struct MAC_ADDR *mac);
int send_arp_eth(unsigned char *buf, int len, struct MAC_ADDR *mac);
int send_bc_eth(unsigned char *buf, int len);
int receive_eth(unsigned char *buf);

#endif // _H_ethernet
