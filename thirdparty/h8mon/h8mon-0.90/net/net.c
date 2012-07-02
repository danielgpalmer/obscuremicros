/** @file
 * @break	Neteork
 *
 * @date	2011.01.22
 * @author	Takashi SHUDO
 */

#include "ethernet.h"
#include "arp.h"
#include "ip.h"
#include "udp.h"
#include "icmp.h"
#include "tprintf.h"
#include "net.h"

//#define DEBUG
#ifdef DEBUG
#define NETPRINTF	eprintf
#else
#define NETPRINTF(x, ...)
#endif

static unsigned char buf[ETH_FRM_LEN];

int init_network(void)
{
	init_eth();
	init_arp();
	init_ip();
	init_udp();

	return 0;
}

int proc_network(void)
{
	long rt;
	struct ETHER_HEADER *ethhd;

	while(1) {
		rt = receive_eth(buf);
		if(rt == 0) break;
		
		ethhd = (struct ETHER_HEADER *)buf;
		
		switch(ethhd->protocol) {
		case ETHER_P_ARP:
			proc_arp(buf, rt);
			break;

		case ETHER_P_IP:
			proc_ip(buf, rt);
			break;
			
		default:
			break;
		}
	}

	return 0;
}
