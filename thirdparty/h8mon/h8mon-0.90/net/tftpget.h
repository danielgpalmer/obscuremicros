/** @file
 * @break	TFTP 受信
 *
 * @date	2011.04.09
 * @author	Takashi SHUDO
 */

#ifndef _H_tftpget
#define _H_tftpget

#include "ip.h"

void init_tftp(void);
int start_tftpget(struct IP_ADDR *ipaddr, char *fname, unsigned char *data);
int proc_tftpget(unsigned char *buf, long size);

int tftp_get(struct IP_ADDR *ipaddr, char *fname, unsigned char *data,
	     long *rsize);

#endif // _H_tftpget
