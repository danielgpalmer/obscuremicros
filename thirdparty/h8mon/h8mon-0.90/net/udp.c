/** @file
 * @break	UDP
 *
 * @date	2011.01.23
 * @author	Takashi SHUDO
 */

#include "udp.h"
#include "tprintf.h"

//#define DEBUG
#ifdef DEBUG
#define UDPPRINTF	tprintf
#else
#define UDPPRINTF(x, ...)
#endif

struct UDP_CONTEXT udp_context[MAX_UDP_CONTEXT];

static unsigned short calc_udp_checksum(struct IP_ADDR *tip, unsigned char *buf,
					int len)
{
	int i;
	unsigned long sum = 0;
	int cnt = (len & 0xfffffffe);
	extern struct IP_ADDR	my_ipaddr;

	// UDP仮想ヘッダ部
	sum += ((unsigned long)my_ipaddr.addr[0]<<8) +
			(unsigned long)my_ipaddr.addr[1];
	sum = (sum & 0xffff) + (sum >> 16);
	sum += ((unsigned long)my_ipaddr.addr[2]<<8) +
			(unsigned long)my_ipaddr.addr[3];
	sum = (sum & 0xffff) + (sum >> 16);
	sum += ((unsigned long)tip->addr[0]<<8) +
			(unsigned long)tip->addr[1];
	sum = (sum & 0xffff) + (sum >> 16);
	sum += ((unsigned long)tip->addr[2]<<8) +
			(unsigned long)tip->addr[3];
	sum = (sum & 0xffff) + (sum >> 16);
	sum += ((unsigned long)0x00<<8) +
			(unsigned long)0x11;
	sum = (sum & 0xffff) + (sum >> 16);
	sum += len;
	sum = (sum & 0xffff) + (sum >> 16);
	
	
	// UDPヘッダ部
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

static void set_udp_header(struct UDP_CONTEXT *udpc, unsigned char *buf,
			   int len)
{
	struct UDP_HEADER *udphdr;

	udphdr = (struct UDP_HEADER *)&buf[UDP_HEADER_TOP];

	udphdr->source	= udpc->dst_port;
	udphdr->dest	= udpc->src_port;
	udphdr->len	= UDP_HEADER_LEN + len;
	udphdr->check	= 0;
	udphdr->check	= calc_udp_checksum(&udpc->ipaddr,
					    (unsigned char *)udphdr,
					    UDP_HEADER_LEN + len);
}


int init_udp(void)
{
	int i;

	for(i=0; i<MAX_UDP_CONTEXT; i++) {
		udp_context[i].used = 0;
	}

	return 0;
}

struct UDP_CONTEXT * create_udp(struct IP_ADDR *taddr, unsigned short port)
{
	int i;
	struct UDP_CONTEXT *udpc;

	for(i=0; i<MAX_UDP_CONTEXT; i++) {
		if(udp_context[i].used == 0) {
			udpc = &udp_context[i];
			
			udpc->used	= 1;
			copy_ipaddr(&udpc->ipaddr.addr, &taddr->addr);
			udpc->src_port = port;
			udpc->dst_port = 54321;	// 取りあえず
			
			return udpc;
		}
	}	
	
	return 0;
}


int close_udp(struct UDP_CONTEXT *udpc)
{
	udpc->used = 0;
	
	return 0;
}


int send_udp(struct UDP_CONTEXT *udpc, unsigned char *buf, int len)
{
	int rt;
	
	set_udp_header(udpc, buf, len);
	
	rt = send_ip(&udpc->ipaddr, IP_PROTCOL_UDP, buf, UDP_HEADER_LEN + len);
	
	return rt;
}

int receive_udp(struct UDP_CONTEXT *udpc, unsigned char **buf)
{
	if(udpc->data_len == 0) {
		return 0;
	} else {
		int rt = udpc->data_len;
		
		UDPPRINTF("UDP Recieve %u\n", rt);
		
		*buf = udpc->rdata;
		
		udpc->data_len = 0;
		
		return rt;
	}
}

void set_srcport_udp(struct UDP_CONTEXT *udpc, unsigned short port)
{
	udpc->src_port = port;
}

void get_target_srcport_udp(struct UDP_CONTEXT *udpc, unsigned short *port)
{
	*port = udpc->dsc_port;
}

int proc_udp(unsigned char *buf, int len)
{
	struct UDP_HEADER *udphdr;
	int i;

	udphdr = (struct UDP_HEADER *)&buf[UDP_HEADER_TOP];
	
	UDPPRINTF("UDP PORT %u\n", udphdr->dest);
	UDPPRINTF("    SRC  %u\n", udphdr->source);
	
	for(i=0; i<MAX_UDP_CONTEXT; i++) {
		if(udp_context[i].used != 0) {
			if(udp_context[i].dst_port == udphdr->dest) {
				// UDPコンテキスト毎にバッファが必要
				udp_context[i].dsc_port = udphdr->source;
				udp_context[i].data_len =
						ip_len(buf) - IP_HEADER_LEN
						- UDP_HEADER_LEN;
			
				udp_context[i].rdata = &buf[UDP_DATA_TOP];
				UDPPRINTF("UDP len %u\n",
					  udp_context[i].data_len);
			}
		}
	}
	
	return 0;
}
