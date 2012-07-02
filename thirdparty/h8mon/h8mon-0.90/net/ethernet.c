/** @file
 * @brief	Ethernet
 *
 * @date	2011.01.15
 * @author	Takashi SHUDO
 */

#include "rtl8019as.h"
#include "ethernet.h"
#include "tprintf.h"
#include "str.h"

//#define DEBUG
#ifdef DEBUG
#define ETHPRINTF	tprintf
#else
#define ETHPRINTF(x, ...)
#endif

static const struct MAC_ADDR bc_macaddr = {
	{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
};

struct MAC_ADDR	my_macaddr;

int init_eth(void)
{
	long rt;

	rt = rtl8019as_ioctl(0, (long)&my_macaddr.addr);
	rt = rtl8019as_ioctl(1, 0);	// 受信バッファクリア
	
#ifdef DEBUG
	tprintf("MAC Address : %02x:%02x:%02x:%02x:%02x:%02x\n",
		my_macaddr.addr[0],
		my_macaddr.addr[1],
		my_macaddr.addr[2],
		my_macaddr.addr[3],
		my_macaddr.addr[4],
		my_macaddr.addr[5]);
#endif
	
	return 0;
}

static int send_data(unsigned char *buf, int len)
{
	long rt;
	
	ETHPRINTF("Ether Send : %d\n", len);

#ifdef DEBUGD
	{
		int i;

		tprintf("D: ");
		for(i=0; i<MAC_LEN; i++) {
			tprintf("%02X ", buf[i]);
		}
		tprintf("\n");
		
		tprintf("S: ");
		for(i=0; i<MAC_LEN; i++) {
			tprintf("%02X ", buf[MAC_LEN + i]);
		}
		tprintf("\n");

		tprintf("P: %02X %02X\n", buf[MAC_LEN + MAC_LEN],
			buf[MAC_LEN + MAC_LEN + 1]);
		
		for(i=ETH_HDR_LEN; i<len; i++) {
			tprintf("%02X ", buf[i]);
		}
		tprintf("\n");
	}
#endif

	rt = rtl8019as_write(buf, ETH_HDR_LEN + len);

	ETHPRINTF("Driver Send : %ld\n", rt);
	
	return rt;
}

int send_eth(unsigned char *buf, int len, struct MAC_ADDR *mac)
{
	struct ETHER_HEADER *ehd;

	ehd = (struct ETHER_HEADER *)buf;
	ehd->protocol = ETHER_P_IP;

	copy_mac(&ehd->dst_mac.addr, mac->addr);
	copy_mac(&ehd->src_mac.addr, &my_macaddr.addr);

	return send_data(buf, len);
}

int send_arp_eth(unsigned char *buf, int len, struct MAC_ADDR *mac)
{
	struct ETHER_HEADER *ehd;

	ehd = (struct ETHER_HEADER *)buf;
	ehd->protocol = ETHER_P_ARP;

	copy_mac(&ehd->dst_mac.addr, mac->addr);
	copy_mac(&ehd->src_mac.addr, &my_macaddr.addr);

	return send_data(buf, len);
}

int send_bc_eth(unsigned char *buf, int len)
{
	struct ETHER_HEADER *ehd;

	ehd = (struct ETHER_HEADER *)buf;
	ehd->protocol = ETHER_P_ARP;

	copy_mac(&ehd->dst_mac.addr, &bc_macaddr);
	copy_mac(&ehd->src_mac.addr, &my_macaddr.addr);

	return send_data(buf, len);
}

int receive_eth(unsigned char *buf)
{
	long rt;
	
	rt = rtl8019as_read(buf, ETH_FRM_LEN);
	if(rt != 0) {
		ETHPRINTF("Ether Receive : %ld\n", rt);
	}
	
	return rt;
}
