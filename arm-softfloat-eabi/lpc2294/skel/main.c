#include <stdio.h>
#include "lpc21xx.h"
#include "uart.h"
#include "config.h"

void Initialize(void);
void feed(void);
/**********************************************************
 MAIN
 **********************************************************/
void main() {
	int j; // loop counter (stack variable)

	// Initialize the system
	Initialize();
	PINSEL1 = 0;
	IODIR0 |= 0x40000000; // P0.30 output
	IOSET0 = 0x00000000;

	//bring UART0 up
	uart0Init(UART_BAUD(115200), UART_8N1, UART_FIFO_8);

	printf("\n\nHello, World!\n");

	// endless loop to toggle the two leds
	while (1) {

		for (j = 0; j < 500000; j++)
			; // wait 500 msec
		IOSET0 = 0x40000000;
		IOCLR0 = 0x00000000;
		for (j = 0; j < 500000; j++)
			; // wait 500 msec
		IOSET0 = 0x00000000;
		IOCLR0 = 0x40000000;
		uart0Putch(0x2e);
	}

}

#define PLOCK 0x400

void Initialize(void) {

	// Setting Multiplier and Divider values
	//PLLCFG=0x23;
	PLLCFG = 0x23;
	feed();

	// Enabling the PLL */	
	PLLCON = 0x1;
	feed();

	// Wait for the PLL to lock to set frequency
	while (!(PLLSTAT & PLOCK))
		;

	// Connect the PLL as the clock source
	PLLCON = 0x3;
	feed();

	// Enabling MAM and setting number of clocks used for Flash memory fetch (4 cclks in this case)
	MAMCR = 0x2;
	MAMTIM = 0x4;

	// Setting peripheral Clock (pclk) to System Clock (cclk)
	VPBDIV = VPBDIV_VAL;

}

void feed(void) {
	PLLFEED = PLL_FEED1;
	PLLFEED = PLL_FEED2;
}
