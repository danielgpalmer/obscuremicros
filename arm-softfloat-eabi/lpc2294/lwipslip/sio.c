/*
 * sio.c
 *
 *  Created on: 30 Apr 2011
 *      Author: daniel
 */

#include "lwip/sio.h"

#include "lpc22xx.h"

#include "sio.h"
#include "interrupts.h"
#include "uart.h"

#define RXRINGSIZE 1024
u8_t rxring[RXRINGSIZE];
u32_t rxring_size = 0;
u32_t rxring_head = 0;
u32_t rxring_tail = 0;



void sio_setuprxint(){

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

u32_t sio_bytesinring(){
	//printf("Ring has %d bytes waiting\n", rxring_size);
	return rxring_size;
}

void sio_rxinthandler(){

	u8_t icode = U1IIR;

	//printf("sio_rxinthandler\n");

	u32_t ch;

	while((ch = uart1Getch()) != -1){

			rxring[rxring_head] = (u8_t) ch; // Stash the byte

			rxring_head++;
			if(rxring_head == RXRINGSIZE){ // Wrap the head around
				rxring_head = 0;
			}

			rxring_size++;
	}


	//printf("Interrupt ID reg %x, RX Ring -- head %d, tail %d\n", icode, rxring_head, rxring_tail);

}



sio_fd_t sio_open(u8_t devnum){
	sio_fd_t fd;
	return fd;
}

void sio_send(u8_t c, sio_fd_t fd){
	uart1Putch(c);
}

u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len){
	printf("sr\n");

	return 0;
}

u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len){

	u32_t bytesread = 0;
	u32_t ch;

	while(rxring_tail != rxring_head && bytesread < len){
		ch = rxring[rxring_tail];
		rxring_tail++;
		rxring_size--;

		if(rxring_tail == RXRINGSIZE){
			rxring_tail = 0;
		}

		*data++ = (u8_t) ch;
		bytesread++;
	}

	return bytesread;
}
