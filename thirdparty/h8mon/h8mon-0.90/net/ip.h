/** @file
 * @brief	IP
 *
 * @date	2011.01.16
 * @author	Takashi SHUDO
 */

#ifndef _H_ip
#define _H_ip

#define IPADDR_LEN 4

#define IP_HEADER_LEN	20

#define IP_PROTCOL_ICMP	1
#define IP_PROTCOL_IP	4
#define IP_PROTCOL_TCP	6
#define IP_PROTCOL_UDP	17

#define copy_ipaddr(dst,src)	memorycopy((dst),(src),IPADDR_LEN)
#define comp_ipaddr(dst,src)	strncomp((char *)(dst),(char *)(src),IPADDR_LEN)

struct IP_ADDR {
	unsigned char addr[IPADDR_LEN];
} __attribute__((packed));

struct IP_HEADER {
	unsigned char	version:4,
			ihl:4;
	unsigned char	tos;
	unsigned short	tot_len;
	unsigned short	id;
	unsigned short	frag_off;
	unsigned char	ttl;
	unsigned char	protocol;
	unsigned short	check;
	struct IP_ADDR	saddr;
	struct IP_ADDR	daddr;
	// option
} __attribute__((packed));


int init_ip(void);
void set_ipaddr(struct IP_ADDR *addr);
void set_netmask(struct IP_ADDR *mask);

unsigned short calc_checksum(unsigned char *buf, int len);
int send_ip(struct IP_ADDR *taddr, unsigned char protocol,
	    unsigned char *buf, int len);
int ip_len(unsigned char *buf);
int proc_ip(unsigned char *buf, int len);

#endif // _H_ip
