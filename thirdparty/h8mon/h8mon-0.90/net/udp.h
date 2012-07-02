/** @file
 * @break	UDP
 *
 * @date	2011.01.23
 * @author	Takashi SHUDO
 */

#ifndef _H_udp
#define _H_udp

#include "ip.h"
#include "ethernet.h"

#define UDP_HEADER_LEN	8	/*8*/

#define UDP_HEADER_TOP	(ETH_HDR_LEN + IP_HEADER_LEN)
#define UDP_DATA_TOP	(ETH_HDR_LEN + IP_HEADER_LEN + UDP_HEADER_LEN)

struct UDP_HEADER {
	unsigned short	source;
	unsigned short	dest;
	unsigned short	len;
	unsigned short	check;
} __attribute__((packed));

struct UDP_CONTEXT {
	unsigned char	used;
	struct	IP_ADDR	ipaddr;
	unsigned short	src_port;	// 送信元 port
	unsigned short	dst_port;	// 送信先 port
	
	unsigned short	dsc_port;	// det_port に受信したときの相手の port

	unsigned short	data_len;
	unsigned char	*rdata;
};

#define MAX_UDP_CONTEXT	1


int init_udp(void);

struct UDP_CONTEXT * create_udp(struct IP_ADDR *taddr, unsigned short port);
int close_udp(struct UDP_CONTEXT *tcpc);

int send_udp(struct UDP_CONTEXT *udpc, unsigned char *buf, int len);
int receive_udp(struct UDP_CONTEXT *udpc, unsigned char **buf);

void set_srcport_udp(struct UDP_CONTEXT *udpc, unsigned short port);
void get_target_srcport_udp(struct UDP_CONTEXT *udpc, unsigned short *port);

int proc_udp(unsigned char *buf, int len);

#endif // _H_udp
