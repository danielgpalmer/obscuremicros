/** @file
 * @brief	RTL8019ASドライバ
 *
 * AKI-H8 3069 LAN ボード対応
 *
 * @date	2011.01.11
 * @authoer	Takashi SHUDO
 */

#include "rtl8019as.h"
#include "tprintf.h"

#define RTL8019_ADDR 0x200000

#define NE2000_CR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x00))
#define RTL8019_RDMAP	(*(volatile unsigned char *)(RTL8019_ADDR + 0x10))
#define RTL8019_RP	(*(volatile unsigned char *)(RTL8019_ADDR + 0x18))

// Page 0
#define NE2000_PSTART	(*(volatile unsigned char *)(RTL8019_ADDR + 0x01))
#define NE2000_PSTOP	(*(volatile unsigned char *)(RTL8019_ADDR + 0x02))
#define NE2000_BNRY	(*(volatile unsigned char *)(RTL8019_ADDR + 0x03))
#define NE2000_TPSR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x04))
#define NE2000_TBCR0	(*(volatile unsigned char *)(RTL8019_ADDR + 0x05))
#define NE2000_TBCR1	(*(volatile unsigned char *)(RTL8019_ADDR + 0x06))
#define NE2000_ISR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x07))
#define NE2000_RSAR0	(*(volatile unsigned char *)(RTL8019_ADDR + 0x08))
#define NE2000_RSAR1	(*(volatile unsigned char *)(RTL8019_ADDR + 0x09))
#define NE2000_RBCR0	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0a))
#define NE2000_RBCR1	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0b))
#define NE2000_RCR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0c))
#define NE2000_TCR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0d))
#define NE2000_DCR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0e))
#define NE2000_IMR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0f))

// Page 1
#define NE2000_PAR0	(*(volatile unsigned char *)(RTL8019_ADDR + 0x01))
#define NE2000_PAR1	(*(volatile unsigned char *)(RTL8019_ADDR + 0x02))
#define NE2000_PAR2	(*(volatile unsigned char *)(RTL8019_ADDR + 0x03))
#define NE2000_PAR3	(*(volatile unsigned char *)(RTL8019_ADDR + 0x04))
#define NE2000_PAR4	(*(volatile unsigned char *)(RTL8019_ADDR + 0x05))
#define NE2000_PAR5	(*(volatile unsigned char *)(RTL8019_ADDR + 0x06))
#define NE2000_CURR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x07))
#define NE2000_MAR0	(*(volatile unsigned char *)(RTL8019_ADDR + 0x08))
#define NE2000_MAR1	(*(volatile unsigned char *)(RTL8019_ADDR + 0x09))
#define NE2000_MAR2	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0a))
#define NE2000_MAR3	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0b))
#define NE2000_MAR4	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0c))
#define NE2000_MAR5	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0d))
#define NE2000_MAR6	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0e))
#define NE2000_MAR7	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0f))

// Page 3
#define RTL8019_9346CR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x01))
#define RTL8019_BPAGE	(*(volatile unsigned char *)(RTL8019_ADDR + 0x02))
#define RTL8019_CONFIG0	(*(volatile unsigned char *)(RTL8019_ADDR + 0x03))
#define RTL8019_CONFIG1	(*(volatile unsigned char *)(RTL8019_ADDR + 0x04))
#define RTL8019_CONFIG2	(*(volatile unsigned char *)(RTL8019_ADDR + 0x05))
#define RTL8019_CONFIG3	(*(volatile unsigned char *)(RTL8019_ADDR + 0x06))
#define RTL8019_TEST	(*(volatile unsigned char *)(RTL8019_ADDR + 0x07))
#define RTL8019_CSNSAV	(*(volatile unsigned char *)(RTL8019_ADDR + 0x08))
#define RTL8019_HLTCLK	(*(volatile unsigned char *)(RTL8019_ADDR + 0x09))
#define RTL8019_INTR	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0b))
#define RTL8019_CONFIG4	(*(volatile unsigned char *)(RTL8019_ADDR + 0x0d))

// CR
#define BIT_CR_P0	(0 << 6)
#define BIT_CR_P1	(1 << 6)
#define BIT_CR_P2	(2 << 6)
#define BIT_CR_P3	(3 << 6)
#define BIT_CR_RD_ABORT	(4 << 3)
#define BIT_CR_RD_WRITE	(2 << 3)
#define BIT_CR_RD_READ	(1 << 3)
#define BIT_CR_TXP	(1 << 2)
#define BIT_CR_STA	(1 << 1)
#define BIT_CR_STP	(1 << 0)

#define BIT_ISR_RDC	(1 << 6)

#define BIT_RCR_MON	(1 << 5)
#define BIT_RCR_PRO	(1 << 4)
#define BIT_RCR_AM	(1 << 3)
#define BIT_RCR_AB	(1 << 2)

#define BIT_TCR_ELB	(2 << 1)
#define BIT_TCR_ILB	(1 << 1)
#define BIT_TCR_NORMAL	(0 << 1)

#define BIT_DCR_F1	(1 << 6)
#define BIT_DCR_F0	(1 << 5)
#define BIT_DCR_LS	(1 << 3)
#define BIT_DCR_BOS	(1 << 1)

#define NE2000_TP_START	0x40
#define NE2000_RP_START	0x46
#define NE2000_RP_STOP	0x80

#define TIMEOUT	500000

static unsigned char macaddr[6];

static int read_data(int addr, int size, unsigned char *buf)
{
	int i;
	unsigned long timer = TIMEOUT;
	
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	NE2000_RBCR0	= size & 0xff;
	NE2000_RBCR1	= (size >> 8) & 0xff;
	NE2000_RSAR0	= addr & 0xff;
	NE2000_RSAR1	= (addr >> 8) & 0xff;
	NE2000_ISR	= BIT_ISR_RDC;
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_READ | BIT_CR_STA;

	for(i=0; i<size; i++) {
		buf[i] = RTL8019_RDMAP;
	}

	while((NE2000_ISR & BIT_ISR_RDC) == 0) {
		if(timer == 0) {
			tprintf("Read timeout\n");
			break;
		}
		timer--;
	}
	
	return size;
}

#define MIN_SIZE	60

static int write_data(unsigned short addr, int size, unsigned char *buf)
{
	int i, tmp = size;
	unsigned long timer = TIMEOUT;
        
	if(size < MIN_SIZE) {
		tmp = MIN_SIZE;
	}
	
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	NE2000_RBCR0	= tmp & 0xff;
	NE2000_RBCR1	= (tmp >> 8) & 0xff;
	NE2000_RSAR0	= addr & 0xff;
	NE2000_RSAR1	= (addr >> 8) & 0xff;
	NE2000_ISR	= BIT_ISR_RDC;
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_WRITE | BIT_CR_STA;
	
	for(i=0; i<size; i++) {
		RTL8019_RDMAP = buf[i];
	}

	if(size < MIN_SIZE) {
		for(i=size; i<MIN_SIZE; i++) {
			RTL8019_RDMAP = 0;
		}
	}
	
	while((NE2000_ISR & BIT_ISR_RDC) == 0) {
		if(timer == 0) {
			tprintf("Write timeout\n");
			break;
		}
		timer--;
	}

	return size;
}

static void enable_int(void)
{
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
//	NE2000_IMR	= 0x01;
}

static void disable_int(void)
{
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STP;
	NE2000_IMR	= 0x00;
}

static int ether_checkintr(void)
{
	unsigned char status;
	
	NE2000_CR = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	status = NE2000_ISR;
	return (status & 0x01) ? 1 : 0;
}

static int clear_irq(void)
{
	if(ether_checkintr()) {
		NE2000_CR  = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
		NE2000_ISR = 0xff;
	}
	
	return 0;
}

int rtl8019as_init(void)
{
	int i;
	unsigned char tmp;
	unsigned char buf[12];
	
	// Software Reset
	tmp = RTL8019_RP;
	RTL8019_RP = tmp;

	for(i=0;i<30000; i++)
		;	// wait

	// デバイスが見つからない場合は登録失敗にする
	if((NE2000_RBCR0 != 'P') || (NE2000_RBCR1 != 'p')) {
		tprintf("Cannot found RTL8019AS.\n");
		return -1;
	}
	   
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STP;	// 停止
	NE2000_DCR	= BIT_DCR_F1 | BIT_DCR_LS | BIT_DCR_BOS;
	NE2000_RBCR0	= 0x00;
	NE2000_RBCR1	= 0x00;
	NE2000_RCR	= BIT_RCR_MON;
	NE2000_TCR	= BIT_TCR_ILB;
	NE2000_TPSR	= NE2000_TP_START;
	NE2000_PSTART	= NE2000_RP_START;
	NE2000_BNRY	= NE2000_RP_START;
	NE2000_PSTOP	= NE2000_RP_STOP;
	NE2000_IMR	= 0x00;
	NE2000_ISR	= 0xff;

	read_data(0, 12, buf);
	
	for(i=0; i<6; i++) {
		macaddr[i] = buf[i * 2];
	}

	tprintf("RTL8019AS Initialized\n");
	tprintf("MAC : %02x:%02x:%02x:%02x:%02x:%02x\n",
		macaddr[0],
		macaddr[1],
		macaddr[2],
		macaddr[3],
		macaddr[4],
		macaddr[5]);
	
	NE2000_CR	= BIT_CR_P1 | BIT_CR_RD_ABORT | BIT_CR_STP;
	NE2000_PAR0	= macaddr[0];
	NE2000_PAR1	= macaddr[1];
	NE2000_PAR2	= macaddr[2];
	NE2000_PAR3	= macaddr[3];
	NE2000_PAR4	= macaddr[4];
	NE2000_PAR5	= macaddr[5];
	NE2000_CURR	= NE2000_RP_START + 1;
	NE2000_MAR0	= 0x00;
	NE2000_MAR1	= 0x00;
	NE2000_MAR2	= 0x00;
	NE2000_MAR3	= 0x00;
	NE2000_MAR4	= 0x00;
	NE2000_MAR5	= 0x00;
	NE2000_MAR6	= 0x00;
	NE2000_MAR7	= 0x00;
	
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STP;
	NE2000_RCR	= BIT_RCR_AM | BIT_RCR_AB | BIT_RCR_PRO;
//	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
//	NE2000_TCR	= BIT_TCR_NORMAL;
	NE2000_IMR	= 0x00;

	clear_irq();

	return 0;
}

static int dispose_data(int addr, int size)
{
	int i;
	unsigned char tmp;
	unsigned long timer = TIMEOUT;
	
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	NE2000_RBCR0	= size & 0xff;
	NE2000_RBCR1	= (size >> 8) & 0xff;
	NE2000_RSAR0	= addr & 0xff;
	NE2000_RSAR1	= (addr >> 8) & 0xff;
	NE2000_CR	= BIT_CR_P0 | BIT_CR_RD_READ | BIT_CR_STA;

	for(i=0; i<size; i++) {
		tmp = RTL8019_RDMAP;
	}

	while((NE2000_ISR & BIT_ISR_RDC) == 0) {
		if(timer == 0) {
			tprintf("Dispose timeout\n");
			break;
		}
		timer--;
	}
	
	return size;
}

static int dispose_rbuf(void)
{
	unsigned char start, curr;
	unsigned char header[4];
	long size;
	
	NE2000_CR = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	start = NE2000_BNRY + 1;
	
	NE2000_CR = BIT_CR_P1 | BIT_CR_RD_ABORT | BIT_CR_STA;
	curr = NE2000_CURR;
	
	if(curr < start)
		curr += NE2000_RP_STOP - NE2000_RP_START;
	if(start == curr)
		return 0;
	if(start == NE2000_RP_STOP)
		start = NE2000_RP_START;
	
	read_data(start * 256, 4, header);
	
	size = ((int)header[3] << 8) + header[2] - 4;
	dispose_data((start * 256) + 4, size);
	
	NE2000_CR = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	if(header[1] == NE2000_RP_START)
		header[1] = NE2000_RP_STOP;
	NE2000_BNRY = header[1] - 1;
	
	return size;
}

int rtl8019as_open(void)
{
	NE2000_TCR	= BIT_TCR_NORMAL;
	enable_int();
	
	return 0;
}

int rtl8019as_close(void)
{
	NE2000_TCR	= BIT_TCR_ILB;
	disable_int();
	
	return 0;
}

long rtl8019as_read(unsigned char *data, long size)
{
	unsigned char start, curr;
	unsigned char header[4];

	NE2000_CR = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	start = NE2000_BNRY + 1;
	
	NE2000_CR = BIT_CR_P1 | BIT_CR_RD_ABORT | BIT_CR_STA;
	curr = NE2000_CURR;
	
	if(curr < start)
		curr += NE2000_RP_STOP - NE2000_RP_START;
	if(start == curr) {
		size = 0;
		goto read_end;
	}
	if(start == NE2000_RP_STOP)
		start = NE2000_RP_START;
	
	read_data(start * 256, 4, header);
	
	size = ((int)header[3] << 8) + header[2] - 4;
	read_data((start * 256) + 4, size, data);
	
	NE2000_CR = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	if(header[1] == NE2000_RP_START)
		header[1] = NE2000_RP_STOP;
	NE2000_BNRY = header[1] - 1;
	
read_end:
	
	return size;
}

long rtl8019as_write(unsigned char *data, long size)
{
	unsigned long timer = TIMEOUT;
	
	write_data(NE2000_TP_START * 256, size, data);
        
	if(size < MIN_SIZE) {
		size = MIN_SIZE;
	}
	
	NE2000_CR    = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_STA;
	NE2000_TBCR0 = size & 0xff;
	NE2000_TBCR1 = (size >> 8) & 0xff;
	NE2000_TPSR  = NE2000_TP_START;
	NE2000_CR    = BIT_CR_P0 | BIT_CR_RD_ABORT | BIT_CR_TXP | BIT_CR_STA;

	while((NE2000_CR & BIT_CR_TXP) != 0) {
		if(timer == 0) {
			tprintf("TX timeout\n");
			break;
		}
		timer--;
	}
	
	return size;
}

int rtl8019as_ioctl(long com, long arg)
{
	switch(com) {
	case 0:	// MACアドレス取得
	{
		int i;

		for(i=0; i<6; i++) {
			((unsigned char *)arg)[i] = macaddr[i];
		}
	}
	break;

	case 1: // 受信バッファクリア
		while(1) {
			if(dispose_rbuf() == 0) break;
		}
		break;
	
	default:
		break;
	}
	
	return 0;
}
