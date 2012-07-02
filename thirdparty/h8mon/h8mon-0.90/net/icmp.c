/** @file
 * @brief	ICMP
 *
 * @date	2011.01.22
 * @author	Takashi SHUDO
 */

#include "ethernet.h"
#include "ip.h"
#include "icmp.h"
#include "tprintf.h"
#include "str.h"

//#define DEBUG
#ifdef DEBUG
#define ICMPPRINTF	eprintf
#else
#define ICMPPRINTF(x, ...)
#endif

static int send_icmp(unsigned char *buf, int len)
{
	struct ETHER_HEADER *ethdr;
	struct IP_HEADER *iphdr;
	struct ICMP_HEADER *icmphdr;
	
	struct MAC_ADDR tmacaddr;
	struct IP_ADDR tipaddr;

	ethdr = (struct ETHER_HEADER *)buf;
	iphdr = (struct IP_HEADER *)&buf[ETH_HDR_LEN];
	icmphdr = (struct ICMP_HEADER *)&buf[ETH_HDR_LEN + IP_HEADER_LEN];

	copy_mac(&tmacaddr.addr, &ethdr->src_mac.addr);

	copy_ipaddr(&tipaddr.addr, &iphdr->saddr.addr);
	copy_ipaddr(&iphdr->saddr.addr, &iphdr->daddr.addr);
	copy_ipaddr(&iphdr->daddr.addr, &tipaddr.addr);

	icmphdr->type		= ICMP_TYPE_PINGREP;
	icmphdr->checksum	= 0;
	icmphdr->checksum	= calc_checksum((unsigned char *)icmphdr,
					len - ETH_HDR_LEN - IP_HEADER_LEN);
	
	send_eth(buf, len - ETH_HDR_LEN, &tmacaddr);
	
	return 0;
}

int proc_icmp(unsigned char *buf, int len)
{
	struct ICMP_HEADER *icmphdr;
	
	icmphdr = (struct ICMP_HEADER *)&buf[ETH_HDR_LEN + IP_HEADER_LEN];
	
	switch(icmphdr->type) {
	case ICMP_TYPE_PINGREQ:
		ICMPPRINTF("ICMP request.\n");
		send_icmp(buf, len);
		break;

	case ICMP_TYPE_PINGREP:
		ICMPPRINTF("ICMP response.\n");
		break;
	}
	
	return 0;
}
