/** @file
 * @brief	IP
 *
 * @date	2011.01.16
 * @author	Takashi SHUDO
 */

#include "ethernet.h"
#include "arp.h"
#include "icmp.h"
#include "ip.h"
#include "udp.h"
#include "str.h"
#include "tprintf.h"

//#define DEBUG
#ifdef DEBUG
#define IPPRINTF	eprintf
#else
#define IPPRINTF(x, ...)
#endif

const struct IP_ADDR def_my_ipaddr = {{192, 168, 1, 123}};
const struct IP_ADDR def_net_mask = {{255, 255, 255, 0}};

struct IP_ADDR my_ipaddr;
struct IP_ADDR net_mask;

static unsigned short id_val;

int init_ip(void)
{
	my_ipaddr = def_my_ipaddr;
	net_mask  = def_net_mask;
	
	id_val = 0;
	
	return 0;
}

void set_ipaddr(struct IP_ADDR *addr)
{
	copy_ipaddr(&my_ipaddr.addr, &addr->addr);
}

void set_netmask(struct IP_ADDR *mask)
{
	copy_ipaddr(&net_mask.addr, &mask->addr);
}

unsigned short calc_checksum(unsigned char *buf, int len)
{
	int i;
	unsigned long sum = 0;
	int cnt = (len & 0xfffffffe);

	for(i=0; i<cnt; i+=2) {
		sum += ((unsigned long)buf[i]<<8) + (unsigned long)buf[i+1];
		sum = (sum & 0xffff) + (sum >> 16);
	}
	
	if(len & 1) {
		sum += ((unsigned long)buf[len-1]<<8);
		sum = (sum & 0xffff) + (sum >> 16);
	}

	return (~sum) & 0xffff;
}

int send_ip(struct IP_ADDR *taddr, unsigned char protocol,
	    unsigned char *buf, int len)
{
	struct IP_HEADER *iphdr;
	struct MAC_ADDR	*tmacaddr;

	iphdr = (struct IP_HEADER *)&buf[ETH_HDR_LEN];

	iphdr->version	= 4;
	iphdr->ihl	= 5;
	iphdr->tos	= 0x00;
	iphdr->tot_len	= len + IP_HEADER_LEN;
	iphdr->id	= id_val;
	iphdr->frag_off	= 0x4000;	// 分割不可
	iphdr->ttl	= 64;		// 生存時間
	iphdr->protocol	= protocol;
	iphdr->check	= 0;
	
	copy_ipaddr(&iphdr->saddr.addr, &my_ipaddr.addr);
	copy_ipaddr(&iphdr->daddr.addr, &taddr->addr);

	iphdr->check	= calc_checksum(&buf[ETH_HDR_LEN], IP_HEADER_LEN);
	
	id_val ++;

	if(search_ipaddr_mac(taddr, &tmacaddr) == 0) {
		IPPRINTF("No ARP cache.\n");
		return 0;
	}
	
	send_eth(buf, IP_HEADER_LEN + len, tmacaddr);
			
	return len;
}

int ip_len(unsigned char *buf)
{
	struct IP_HEADER *iphdr;
	
	iphdr = (struct IP_HEADER *)&buf[ETH_HDR_LEN];

	return iphdr->tot_len;
}

int proc_ip(unsigned char *buf, int len)
{
	struct ETHER_HEADER *ehd;
	struct IP_HEADER *iphdr;
	
	iphdr = (struct IP_HEADER *)&buf[ETH_HDR_LEN];
	
	if(comp_ipaddr(&iphdr->daddr.addr, &my_ipaddr.addr) != 0) {
		// 自IP以外
		return 0;
	}

	ehd = (struct ETHER_HEADER *)buf;
	cache_arp(&iphdr->saddr, &ehd->src_mac);

	IPPRINTF("IP_LEN %d.\n", ip_len(buf));
	
	switch(iphdr->protocol) {
	case IP_PROTCOL_ICMP:
		IPPRINTF("ICMP receive.\n");
		proc_icmp(buf, len);
		break;

	case IP_PROTCOL_UDP:
		IPPRINTF("UDP receive.\n");
		proc_udp(buf, len);
		break;

	case IP_PROTCOL_TCP:
		IPPRINTF("TCP receive.\n");
		break;

	default:
		break;
	}

	return 0;
}
