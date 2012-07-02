/** @file
 * @break	ARP
 *
 * @date	2011.01.15
 * @author	Takashi SHUDO
 */

#include "ethernet.h"
#include "arp.h"
#include "str.h"
#include "tprintf.h"

//#define DEBUG
#ifdef DEBUG
#define ARPPRINTF	tprintf
#else
#define ARPPRINTF(x, ...)
#endif

#define MAX_ARPREP_WAIT	3000	// 3000(ms)

extern struct MAC_ADDR	my_macaddr;
extern struct IP_ADDR	my_ipaddr;

static const struct MAC_ADDR zero_macaddr = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};


static unsigned char arp_buf[ETH_HDR_LEN+ARP_MES_LEN];

struct ARP_TABLE	arp_cache[MAX_ARP_CACHE];

#ifdef DEBUG
void dump_arp_cache(void)
{
	int i;

	tprintf("IP                MAC\n");
	for(i=0; i<MAX_ARP_CACHE; i++) {
		if(arp_cache[i].use != 0) {
			tprintf("%3d.%3d.%3d.%3d   ",
				arp_cache[i].ipaddr.addr[0],
				arp_cache[i].ipaddr.addr[1],
				arp_cache[i].ipaddr.addr[2],
				arp_cache[i].ipaddr.addr[3]);
			tprintf("%02X:%02X:%02X:%02X:%02X:%02X   ",
				arp_cache[i].macaddr.addr[0],
				arp_cache[i].macaddr.addr[1],
				arp_cache[i].macaddr.addr[2],
				arp_cache[i].macaddr.addr[3],
				arp_cache[i].macaddr.addr[4],
				arp_cache[i].macaddr.addr[5]);
		}
	}
}
#endif

int init_arp(void)
{
	int i;

	for(i=0; i<MAX_ARP_CACHE; i++) {
		arp_cache[i].use = 0;	// 0は未使用
	}

	return 0;
}

static int send_arp_reply(struct MAC_ADDR *tmac, struct IP_ADDR *tipaddr)
{
	struct ARP_HEADER *arphdr;
	int rt;

	arphdr = (struct ARP_HEADER *)&arp_buf[ETH_HDR_LEN];
	
	arphdr->hardware	= ARP_HARDWARE_ETHER;
	arphdr->protocol	= ETHER_P_IP;
	arphdr->macaddr_size	= MAC_LEN;
	arphdr->ipaddr_size	= IPADDR_LEN;
	arphdr->operation	= ARP_OPERATION_REPLY;

	copy_mac(&arphdr->sender_macaddr.addr, &my_macaddr.addr);
	copy_ipaddr(&arphdr->sender_ipaddr.addr, &my_ipaddr.addr);
	copy_mac(&arphdr->target_macaddr.addr, &tmac->addr);
	copy_ipaddr(&arphdr->target_ipaddr.addr, &tipaddr->addr);
	
	rt = send_arp_eth(arp_buf, ARP_MES_LEN, tmac);

	return rt;
}

int send_arp_request(struct IP_ADDR *ipaddr)
{
	struct ARP_HEADER *arphdr;
	int rt;

	ARPPRINTF("ARP Request\n");
	
	arphdr = (struct ARP_HEADER *)&arp_buf[ETH_HDR_LEN];
	
	arphdr->hardware	= ARP_HARDWARE_ETHER;
	arphdr->protocol	= ETHER_P_IP;
	arphdr->macaddr_size	= MAC_LEN;
	arphdr->ipaddr_size	= IPADDR_LEN;
	arphdr->operation	= ARP_OPERATION_REQUEST;

	copy_mac(&arphdr->sender_macaddr.addr, &my_macaddr.addr);
	copy_ipaddr(&arphdr->sender_ipaddr.addr, &my_ipaddr.addr);
	copy_mac(&arphdr->target_macaddr.addr, &zero_macaddr);
	copy_ipaddr(&arphdr->target_ipaddr.addr, &ipaddr->addr);
	
	rt = send_bc_eth(arp_buf, ARP_MES_LEN);

	return rt;
}


static int search_ipaddr_cache(struct IP_ADDR *ipaddr,
			       struct MAC_ADDR **macaddr)
{
	int i;
	
	for(i=0; i<MAX_ARP_CACHE; i++) {
		if(arp_cache[i].use != 0) {
			if(comp_ipaddr(&ipaddr->addr,
				&arp_cache[i].ipaddr.addr) == 0) {
				*macaddr = &arp_cache[i].macaddr;
				return 1;
			}
		}
	}

	return 0;
}


#ifndef MULTITASK

#include "conio.h"
#include "nettimer.h"
#define MAX_ARPREQ_RETRY	4

int wait_arp_reply(struct IP_ADDR *ipaddr, struct MAC_ADDR **macaddr)
{
	long rt;
	struct ETHER_HEADER *ethhd;
	
	ARPPRINTF("ARP Wait REPLY\n");
	
	start_nettimer();
	
	while(1) {
		char rd;
		
		if(cgetc(&rd) == 0) {
			if(rd == 0x03) {
				tprintf("Abort\n");
				return -1;
			}
		}
		
		if(get_nettimer() >= MAX_ARPREP_WAIT) {
			return 0;
		}

		rt = receive_eth(arp_buf);
		if(rt == 0) continue;
		
		ethhd = (struct ETHER_HEADER *)arp_buf;
		
		switch(ethhd->protocol) {
		case ETHER_P_ARP:
			if(proc_arp(arp_buf, rt) == ARP_OPERATION_REPLY) {
				if(search_ipaddr_cache(ipaddr, macaddr)) {
					ARPPRINTF("ARP Wait REPLY done\n");
					return 1;
				}
			}
			break;
		}
	}
	
	ARPPRINTF("ARP REPLY timeoutT\n");
	return 0;
}
#endif

int search_ipaddr_mac(struct IP_ADDR *ipaddr,
			     struct MAC_ADDR **macaddr)
{
	int i;
	
	if(search_ipaddr_cache(ipaddr, macaddr)) {
		return 1;
	}
	
	// キャッシュに無い場合 arp リクエスト

	for(i=0; i<MAX_ARPREQ_RETRY; i++) {
		int rt;
		
		send_arp_request(ipaddr);

		rt = wait_arp_reply(ipaddr, macaddr);
		if(rt > 0) {
			return 1;
		} else if(rt < 0) {
			return 0;
		}
	}

	return 0;
}


int cache_arp(struct IP_ADDR *ipaddr, struct MAC_ADDR *macaddr)
{
	int i;
	struct MAC_ADDR *tmac;

	if(search_ipaddr_cache(ipaddr, &tmac) != 0) {
		// 既にキャッシュ済み
		if(comp_mac(macaddr, tmac) == 0) {
			// MACも同じ
			return 0;
		}
	}
	
	for(i=0; i<MAX_ARP_CACHE; i++) {
		if(arp_cache[i].use == 0) {
			arp_cache[i].use = 1;
			copy_ipaddr(&arp_cache[i].ipaddr.addr, &ipaddr->addr);
			copy_mac(&arp_cache[i].macaddr.addr, &macaddr->addr);
			return 0;
		} else {
			if(comp_ipaddr(&ipaddr->addr,
				       &arp_cache[i].ipaddr.addr) == 0) {
				copy_mac(&arp_cache[i].macaddr.addr,
					 &macaddr->addr);
				return 0;
			}
		}
	}

	return 0;
}

int proc_arp(unsigned char *buf, int len)
{
	struct ARP_HEADER *arphdr;
	struct IP_ADDR ipaddr;
	struct MAC_ADDR macaddr;

	arphdr = (struct ARP_HEADER *)&buf[ETH_HDR_LEN];
	
	ARPPRINTF("ARP : %02x %02x %02x %02x %02x %02x\n",
		  arphdr->sender_macaddr.addr[0],
		  arphdr->sender_macaddr.addr[1],
		  arphdr->sender_macaddr.addr[2],
		  arphdr->sender_macaddr.addr[3],
		  arphdr->sender_macaddr.addr[4],
		  arphdr->sender_macaddr.addr[5]);
	
	if(arphdr->hardware != ARP_HARDWARE_ETHER)	return 0;
	if(arphdr->protocol != ETHER_P_IP)	return 0;

	ARPPRINTF("Receive ARP\n");
	
	switch(arphdr->operation) {
	case ARP_OPERATION_REPLY:
		ARPPRINTF("ARP REPLY\n");
		copy_mac(&macaddr.addr, &arphdr->sender_macaddr.addr);
		copy_ipaddr(&ipaddr.addr, &arphdr->sender_ipaddr.addr);
		
		cache_arp(&ipaddr, &macaddr);
		return ARP_OPERATION_REPLY;
		break;
		
	case ARP_OPERATION_REQUEST:
		ARPPRINTF("ARP REQUEST\n");
		
		copy_mac(&macaddr.addr, &arphdr->sender_macaddr.addr);
		copy_ipaddr(&ipaddr.addr, &arphdr->sender_ipaddr.addr);
		
		if(comp_ipaddr(&arphdr->target_ipaddr.addr, &my_ipaddr.addr))
			    break;
		
		cache_arp(&ipaddr, &macaddr);
		send_arp_reply(&macaddr, &ipaddr);
		return ARP_OPERATION_REQUEST;
		break;
		
	default:
		break;
	}
	
	return 0;
}
