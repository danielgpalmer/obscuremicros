/** @file
 * @brief	ICMP
 *
 * @date	2011.01.22
 * @author	Takashi SHUDO
 */

#ifndef _H_icmp
#define _H_icmp

#define ICMP_HDR_LEN	4

struct ICMP_HEADER {
	unsigned char	type;
#define ICMP_TYPE_PINGREQ	0x08
#define ICMP_TYPE_PINGREP	0x00

	unsigned char	code;
	unsigned short	checksum;
} __attribute__((packed));


int proc_icmp(unsigned char *buf, int len);

#endif // _H_icmp
