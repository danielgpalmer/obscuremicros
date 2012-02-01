#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "lpc22xx.h"
#include "uart.h"
#include "config.h"
#include "interrupts.h"
#include "timer.h"
#include <time.h>

void initialize(void);
void feed(void);

void timerhandler(void) __attribute__ ((interrupt("IRQ")));

void timerhandler(void) {

	static bool on = false;

	if (on) {
		IOSET0 = 0x40000000;
		IOCLR0 = 0x00000000;
	}
	else {
		IOSET0 = 0x00000000;
		IOCLR0 = 0x40000000;
	}

	on = !on;

	T0IR = 0x0001;
	VICVectAddr = 0;

}

void setuptimer() {
	printf("config timer 0..\n");

	timer_reset(0);

	T0PR = 0xFFFF; // timer0 prescaler
	T0MR0 = 0x00FF; // match

	T0MCR = TIMER_MATCHCONTROL_MR0I | TIMER_MATCHCONTROL_MR0R;

	VICIntEnable = INTERRUPT_MASK_TIMER0; // enable timer0 interrupt
	VICVectCntl0 = INTERRUPT_CHANNEL_ENABLE | INTERRUPT_CHANNEL_TIMER0;
	VICVectAddr0 = (uint32_t) timerhandler;

	enableIRQ();

	timer_start(0);

}

void main() {

	initialize();
	interrupts_reset();
	PINSEL1 = 0;
	IODIR0 |= 0x40000000; // P0.30 output
	IOSET0 = 0x00000000;

	//bring UART0 up
	uart0Init(UART_BAUD(115200), UART_8N1, UART_FIFO_8);

	printf("Hello, World!\n");

	setuptimer();

	rtc_start();

	while (1) {

		for (unsigned i = 0; i < 500000; i++) {

		}
		rtc_dumpregisters();
		timer_t timer = time(NULL);
		char* timestring = ctime(&timer);
		printf("%s\n", timestring);
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

