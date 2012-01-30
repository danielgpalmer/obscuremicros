#include <stdio.h>
#include "lpc22xx.h"
#include "uart.h"
#include "config.h"
#include "interrupts.h"

void initialize(void);
void feed(void);

void timerhandler(void) __attribute__ ((interrupt("IRQ")));

void timerhandler(void) {

	T0IR = 0x0001;
	VICVectAddr = 0;
	printf("t\n");

}

void setuptimer() {
	printf("Starting timer0..\n");
	dumpInterruptRegisters();

	VICIntSelect = 0;
	VICIntEnClr = 0xFFFF;

	T0TCR = 0x0003; // enable and reset the counter.. hold it.
	T0IR = 0x0001;
	T0PR = 0xFFFF; // timer0 prescaler
	T0MR0 = 0x0001; // match
	T0MCR = 0x0003;
	//T0MR1 = 0x01FE;
	//T0MCR = 0x0019;  // match; fire interrupt, reset counter.
	VICIntEnable |= 0x0010; // enable timer0 interrupt

	VICVectCntl0 = 0x0024;
	VICVectAddr0 = (uint32_t) timerhandler;

	//VICIntSelect = 0x0010; // make timer0 interrupts FIQ
	T0TCR = 0x0001; // go!

	enableIRQ();
	//enableFIQ();

	dumpInterruptRegisters();

}

void main() {

	initialize();
	PINSEL1 = 0;
	IODIR0 |= 0x40000000; // P0.30 output
	IOSET0 = 0x00000000;

	//bring UART0 up
	uart0Init(UART_BAUD(115200), UART_8N1, UART_FIFO_8);

	printf("Hello, World!\n");

	setuptimer();

	// endless loop to toggle the led
	while (1) {
		for (int j = 0; j < 500000; j++)
			; // wait 500 msec
		IOSET0 = 0x40000000;
		IOCLR0 = 0x00000000;
		for (int j = 0; j < 500000; j++)
			; // wait 500 msec
		IOSET0 = 0x00000000;
		IOCLR0 = 0x40000000;
		dumpInterruptRegisters();
		timer_dumpregisters();
	}

}

#define PLOCK 0x400

void initialize(void) {

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

