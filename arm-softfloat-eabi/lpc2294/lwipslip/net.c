/*
 * net.c
 *
 *  Created on: 30 Apr 2011
 *      Author: daniel
 */

#include "lpc22xx.h"
#include "uart.h"
#include "net.h"
#include "interrupts.h"

// LWIP
#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "lwip/ip_frag.h"
#include "lwip/ip.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/stats.h"
#include "lwip/sio.h"
#include "lwip/pbuf.h"
#include "netif/etharp.h"
#include "netif/slipif.h"

#include "echo.h"

#define IPADDRESS "192.168.93.2"
#define GWADDRESS "192.168.93.1"

// Our network interface structure
static struct netif slip_netif;

void net_init() {

	struct ip_addr myip, gw_addr, netmask;

	lwip_init();

	inet_aton(IPADDRESS, &myip);
	inet_aton(GWADDRESS, &gw_addr);
	inet_aton("255.255.255.255", &netmask);

	if (netif_add(&slip_netif, &myip, &netmask, &gw_addr, NULL, slipif_init, ip_input) == NULL) {
		printf("ERROR!!!\n");
	}

	netif_set_default(&slip_netif);
	netif_set_up(&slip_netif);

	echo_init();

}

void net_loop() {

	static int loop = 0;
	//slipif_poll(&slip_netif);
	slipif_process_rxqueue(&slip_netif);

	loop++;
	if (loop == 100) {
		tcp_tmr();
		loop = 0;
	}

}

#define BUFFERSIZE 64

static u8_t rxbuffer[BUFFERSIZE];

void sio_setuprxint() {

	printf("sio_setiprxint()\n");

	printf("-- Interrupt setup before --\n");
	dumpInterruptRegisters();

	U1IER = 0x1; // Enabled the RX data ready interrupt
	VICIntEnable |= INTCHAN_UART1;
	VICIntSelect = INTCHAN_UART1;

	enableFIQ();

	printf("-- Interrupt setup after --\n");
	dumpInterruptRegisters();

}

void sio_rxinthandler() {
	u8_t icode = U1IIR;
	u8_t pos = 0;
	int ch;
	while ((ch = uart1Getch()) != -1) {
		rxbuffer[pos] = (u8_t) ch;
		pos++;
	}
	if (pos > 0) {
		slipif_received_bytes(&slip_netif, rxbuffer, pos);
	}

}

sio_fd_t sio_open(u8_t devnum) {
	sio_fd_t fd;
	return fd;
}

void sio_send(u8_t c, sio_fd_t fd) {
	uart1Putch(c);
}

// unused

u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len) {
	return 0;
}
