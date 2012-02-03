/*
 * net.c
 *
 *  Created on: 30 Apr 2011
 *      Author: daniel
 */

#include "uart.h"
#include "net.h"


// LWIP
#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/ip_frag.h"
#include "lwip/ip.h"
#include "lwip/netif.h"
#include "lwip/init.h"
#include "lwip/stats.h"
#include "lwip/sio.h"
#include "lwip/pbuf.h"
#include "netif/etharp.h"
#include "netif/slipif.h"

#include "sio.h"
#include "echo.h"

// Our network interface structure
static struct netif slip_netif;

void net_init(){

	struct ip_addr myip, gw_addr, netmask;

	lwip_init();

	inet_aton("192.168.69.2", &myip);
	inet_aton("192.168.69.1", &gw_addr);
	inet_aton("255.255.255.255", &netmask);

	if (netif_add(&slip_netif, &myip, &netmask, &gw_addr, NULL, slipif_init, ip_input) == NULL) {
		printf("ERROR!!!\n");
	}

	netif_set_default(&slip_netif);
	netif_set_up(&slip_netif);

	echo_init();

}

void net_loop(){

	//printf("net_loop()\n");

	static int i = 0;

	slipif_poll(&slip_netif);

	if(i == 10){
		tcp_tmr();
		i = 0;
	}
	else {
		i++;
	}

}
