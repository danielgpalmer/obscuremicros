#include "lpc22xx.h"
#include "uart.h"
#include "config.h"
#include <stdbool.h>
#include <stdio.h>

#include "exceptions.h"
#include "interrupts.h"
#include "net.h"

#include <lwip/inet.h>
#include "ping.h"

void starttimer(){

	printf("Starting timer0..\n");
	dumpInterruptRegisters();

	T0TCR = 0x0003; // enable and reset the counter.. hold it.
	T0IR = 0x0001;
	T0PR = 0xFFFF; // timer0 prescaler
	T0MR0 = 0x0001; // match
	T0MCR = 0x0003;
	//T0MR1 = 0x01FE;
	//T0MCR = 0x0019;  // match; fire interrupt, reset counter.
	VICIntEnable |= 0x0010; // enable timer0 interrupt

	VICVectCntl0 = 0x0024;
	VICVectAddr0=(uint32_t) IRQ_Routine;


	//VICIntSelect = 0x0010; // make timer0 interrupts FIQ
	T0TCR = 0x0001; // go!

	enableIRQ();
	//enableFIQ();

	dumpInterruptRegisters();
}


void main(){ 

	VICIntSelect = 0;
	VICIntEnClr = 0xFFFF;

	
	// Initialize the system
	Initialize();
	PINSEL1=0;
	IODIR0 |= 0x60000000;	// P0.29 & P0.30 output
	IOSET0 =  0x00000000;
 
	//bring UART0 up
	uart0Init(UART_BAUD(115200), UART_8N1, UART_FIFO_8);
	uart1Init(UART_BAUD(115200), UART_8N1, UART_FIFO_14);
	

	printf("hello\n");

	while(1){
	}

}

/**********************************************************
                      Initialize
**********************************************************/

#define PLOCK 0x400

void Initialize(void)  {

   
	// Setting Multiplier and Divider values
  	//PLLCFG=0x23;
  	PLLCFG=0x23;
	feed();
  
	// Enabling the PLL */	
	PLLCON=0x1;
	feed();
  
	// Wait for the PLL to lock to set frequency
	while(!(PLLSTAT & PLOCK)) ;
  
	// Connect the PLL as the clock source
	PLLCON=0x3;
	feed();
  


	//PINSEL2 = 0x0F000924;
	//BCFG0=0x1000348F;
	//BCFG1=0x2000040F;


	//MEMMAP = 2;

	// Enabling MAM and setting number of clocks used for Flash memory fetch (4 cclks in this case)
	MAMCR=0x2;
	MAMTIM=0x4;
  
	//clear external SRAM
	//uint32_t *esram = (uint32_t) 0x81000000;
	//while(esram < 0x81040000){
	//	*esram = 0;
	//	esram++;
	//}


	// Setting peripheral Clock (pclk) to System Clock (cclk)
	VPBDIV = VPBDIV_VAL;


/*



*/
}


void feed(void)
{
  PLLFEED = PLL_FEED1;
  PLLFEED = PLL_FEED2;
}


