/** @file
 * @break	TFTP 受信
 *
 * @date	2011.04.09
 * @author	Takashi SHUDO
 */

#include "ethernet.h"
#include "udp.h"
#include "net.h"
#include "tftp.h"
#include "tftpget.h"
#include "str.h"
#include "tprintf.h"
#include "nettimer.h"

//#define DEBUG
#ifdef DEBUG
#define TFGPRINTF	tprintf
#else
#define TFGPRINTF(x, ...)
#endif

#define DEF_LOAD_ADDR	0x500000
#define TIMEOUT	3000

static struct UDP_CONTEXT *uc;
static int rcvport;
static unsigned short blk_num;
static unsigned char *rcv_data;
static long rcv_size;

static unsigned char tmp[ETH_FRM_LEN];

static void save_data(unsigned char *data, int len)
{
	int i;

	for(i=0; i<len; i++) {
		*rcv_data = *data;
		rcv_data ++;
		data ++;
	}
	
	rcv_size += len;
}

void init_tftp(void)
{
	blk_num = 1;
	rcvport = 0;
	rcv_size = 0;
}

int start_tftpget(struct IP_ADDR *ipaddr, char *fname, unsigned char *data)
{
	unsigned char *buf = &tmp[UDP_DATA_TOP];
	long len = 0;
	int rt;
	
	rcv_data = data;
	
	uc = create_udp(ipaddr, TFTP_PORT);
	
	buf[0] = 0;
	buf[1] = TFTP_OPC_READREQ;
	len = 2;

	strcopy((char *)&buf[len], fname);
	len += (strleng(fname) + 1);

	strcopy((char *)&buf[len], TFTP_MODE_BIN);
	len += (strleng(TFTP_MODE_BIN) + 1);
	
	rt = send_udp(uc, tmp, len);

	return rt;
}

int proc_tftpget(unsigned char *buf, long size)
{
	unsigned short opcode;
	unsigned short rcv_blk;
	long wsize = size - 4;

	opcode = ((int)buf[0]<<8) + buf[1];
	
	if(opcode == TFTP_OPC_ERROR) {
		tprintf("Receive error responce(%u)\n", opcode);
		return -1;
	}
	
	if(opcode != TFTP_OPC_DATA) {
		tprintf("Invarid responce(%u)\n", opcode);
		return 0;
	}
	
	rcv_blk = ((int)buf[2]<<8) + buf[3];
	if(rcv_blk != blk_num) {
		tprintf("Invarid block(%u)\n", rcv_blk);
		return 0;
	} else {
		unsigned char *ackbuf = &tmp[UDP_DATA_TOP];
		
		if(rcvport == 0) {
			// 最初の受信データ
			unsigned short port;
			tprintf("Start\n");
			get_target_srcport_udp(uc, &port);
			set_srcport_udp(uc, port);
			tprintf("Port %u\n", port);
			rcvport = 1;
		}
		
		TFGPRINTF("Block %u\n", rcv_blk);
		tprintf(".");
		
		blk_num ++;

		save_data(&buf[4], wsize);
		
		// ACK
		ackbuf[0] = 0;
		ackbuf[1] = TFTP_OPC_ACK;
		ackbuf[2] = (rcv_blk >> 8);
		ackbuf[3] = (rcv_blk & 0xff);
		send_udp(uc, tmp, 4);
		
		if(wsize < TFTP_BLK_SIZE) {
			TFGPRINTF("Last block\n");
			return 1;
		}
	}
	
	return 0;
}


#include "conio.h"

int tftp_get(struct IP_ADDR *ipaddr, char *fname, unsigned char *data,
	     long *rsize)
{
	int rt;
	
	init_tftp();
	
	rt = start_tftpget(ipaddr, fname, data);

	if(rt == 0) {
		tprintf("Cannot connect %d.%d.%d.%d\n",
			ipaddr->addr[0],
			ipaddr->addr[1],
			ipaddr->addr[2],
			ipaddr->addr[3]);
		return -1;
	}
	
	start_nettimer();
	
	while(1) {
		long size;
		unsigned char *buf;
		char rd;
		
		if(cgetc(&rd) == 0) {
			if(rd == 0x03) {
				tprintf("Abort\n");
				return -1;
			}
		}
		
		proc_network();
		size = receive_udp(uc, &buf);

		if(size == 0) {
			if(get_nettimer() >= TIMEOUT) {
				tprintf("TFTP Time Out\n");
				break;
			}
			continue;
		}

		start_nettimer();
		
		rt = proc_tftpget(buf, size);

		if(rt < 0) {
			tprintf("Abort\n");
			break;
		} else if(rt > 0) {
			tprintf("\nDone\n");
			break;
		}
	}

	tprintf("Receive size = %ld\n", rcv_size);

	close_udp(uc);

	*rsize = rcv_size;

	if(rt > 0) {
		return 0;
	} else {
		return -1;
	}
}
