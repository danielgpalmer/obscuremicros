/******************************************************************************
 *
 * $RCSfile: $
 * $Revision: $
 *
 * This module provides interface routines to the LPC ARM UARTs.
 * Copyright 2004, R O SoftWare
 * No guarantees, warrantees, or promises, implied or otherwise.
 * May be used for hobby or commercial purposes provided copyright
 * notice remains intact.
 *
 * reduced to see what has to be done for minimum UART-support by mthomas
 *****************************************************************************/

// #warning "this is a reduced version of the R O Software code"

#include "lpc21xx.h"
#include "uart.h"

/* on LPC210x: UART0 TX-Pin=P0.2, RX-Pin=P0.1 
   PINSEL0 has to be set to "UART-Function" = Function "01" 
   for Pin 0.0 and 0.1 */
   
#define PINSEL_BITPIN0  0
#define PINSEL_BITPIN1  2
#define PINSEL_BITPIN8	16
#define PINSEL_BITPIN9  18
// #define PINSEL_BITPIN2  4
#define PINSEL_FIRST_ALT_FUNC   1
// #define PINSEL_SECOND_ALT_FUNC   2

// Values of Bits 0-3 in PINSEL to activate UART0
#define UART0_PINSEL    ((PINSEL_FIRST_ALT_FUNC<<PINSEL_BITPIN0)|(PINSEL_FIRST_ALT_FUNC<<PINSEL_BITPIN1))
#define UART1_PINSEL    ((PINSEL_FIRST_ALT_FUNC<<PINSEL_BITPIN8)|(PINSEL_FIRST_ALT_FUNC<<PINSEL_BITPIN9))
// Mask of Bits 0-4
#define UART0_PINMASK      (0x0000000F)    /* PINSEL0 Mask for UART0 */
#define UART1_PINMASK      (0x000F0000)    /* PINSEL0 Mask for UART1 */

// U0_LCR devisor latch bit 
#define UART0_LCR_DLAB  7
#define UART1_LCR_DLAB  7

/*    baudrate divisor - use UART_BAUD macro
 *    mode - see typical modes (uart.h)
 *    fmode - see typical fmodes (uart.h)
 *    NOTE: uart0Init(UART_BAUD(9600), UART_8N1, UART_FIFO_8); 
 */
void uart0Init(uint16_t baud, uint8_t mode, uint8_t fmode)
{
  // setup Pin Function Select Register (Pin Connect Block) 
  // make sure old values of Bits 0-4 are masked out and
  // set them according to UART0-Pin-Selection
  PINSEL0 = (PINSEL0 & ~UART0_PINMASK) | UART0_PINSEL;

  U0IER = 0x00;             // disable all interrupts
  U0IIR = 0x00;             // clear interrupt ID register
  U0LSR = 0x00;             // clear line status register

  // set the baudrate - DLAB must be set to access DLL/DLM
  U0LCR = (1<<UART0_LCR_DLAB); // set divisor latches (DLAB)
  U0DLL = (uint8_t)baud;         // set for baud low byte
  U0DLM = (uint8_t)(baud >> 8);  // set for baud high byte
  
  // set the number of characters and other
  // user specified operating parameters
  // Databits, Parity, Stopbits - Settings in Line Control Register
  U0LCR = (mode & ~(1<<UART0_LCR_DLAB)); // clear DLAB "on-the-fly"
  // setup FIFO Control Register (fifo-enabled + xx trig) 
  U0FCR = fmode;
}
/*
 
void LPC210xInitUART1()
{
  //Set pins for use with UART
  PINSEL0_bit.P0_8=0x1; // Set pin function to TxD (UART1)
  PINSEL0_bit.P0_9=0x1; // Set pin function to RxD (UART1)

  //Set the FIFO enable bit in the FCR register. This bit must be set for
  //proper UART operation.
  U1FCR_bit.FCRFE = 1;

  //Set baudrate
  U1LCR_bit.DLAB = 1;
  U1DLL = BAUDRATEDIVISOR & 0x00ff;
  U1DLM = (BAUDRATEDIVISOR >> 8) & 0x00ff;
  U1LCR_bit.DLAB = 0;

  //Set mode
  U1LCR_bit.WLS = 0x3;   //8 bit word length
  U1LCR_bit.SBS = 0x0;   //1 stop bit
  U1LCR_bit.PE  = 0x0;   //No parity

  //Enable UART1 interrupts
  U1IER_bit.RDAIE  = 1;  //Enable byte received interrupt
  U1IER_bit.THREIE = 1;  //Enable tx buf empty interrupt
}
*/
void uart1Init(uint16_t baud, uint8_t mode, uint8_t fmode)
{
  // setup Pin Function Select Register (Pin Connect Block) 
  // make sure old values of Bits 0-4 are masked out and
  // set them according to UART0-Pin-Selection
  unsigned int tempval = &(PINSEL0);
  PINSEL0 = (PINSEL0 & ~UART1_PINMASK) | UART1_PINSEL;

  U1IER = 0x00;             // disable all interrupts
  U1IIR = 0x00;             // clear interrupt ID register
  U1LSR = 0x00;             // clear line status register

  // set the baudrate - DLAB must be set to access DLL/DLM
  U1LCR = (1<<UART1_LCR_DLAB); // set divisor latches (DLAB)
  U1DLL = (uint8_t)baud;         // set for baud low byte
  U1DLM = (uint8_t)(baud >> 8);  // set for baud high byte
  
  // set the number of characters and other
  // user specified operating parameters
  // Databits, Parity, Stopbits - Settings in Line Control Register
  U1LCR = (mode & ~(1<<UART1_LCR_DLAB)); // clear DLAB "on-the-fly"
  // setup FIFO Control Register (fifo-enabled + xx trig) 
  U1FCR = fmode;
}

int uart0Putch(int ch)
{
  while (!(U0LSR & ULSR_THRE))          // wait for TX buffer to empty
    continue;                           // also either WDOG() or swap()

  U0THR = (uint8_t)ch;  // put char to Transmit Holding Register
  return (uint8_t)ch;      // return char ("stdio-compatible"?)
}


//
void uart_putc(unsigned char ch)
{
  while (!(U0LSR & ULSR_THRE))          // wait for TX buffer to empty
    continue;                           // also either WDOG() or swap()

  U0THR = ch;  // put char to Transmit Holding Register
}

unsigned char uart_getc(void)
{
  while ( !(U0LSR & ULSR_RDR) ); // wait for character
  
  return U0RBR;                // return character
}

//

int uart1Putch(int ch)
{
  while (!(U1LSR & ULSR_THRE))          // wait for TX buffer to empty
    continue;                           // also either WDOG() or swap()

  U1THR = (uint8_t)ch;  // put char to Transmit Holding Register
  return (uint8_t)ch;      // return char ("stdio-compatible"?)
}

const char *uart0Puts(const char *string)
{
	char ch;
	
	while ((ch = *string)) {
		if (uart0Putch(ch)<0) break;
		string++;
	}
	
	return string;
}

const char *uart1Puts(const char *string)
{
	char ch;
	
	while ((ch = *string)) {
		if (uart1Putch(ch)<0) break;
		string++;
	}
	return string;
}


int uart0TxEmpty(void)
{
  return (U0LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

int uart1TxEmpty(void)
{
  return (U1LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}


void uart0TxFlush(void)
{
  U0FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo
}

void uart1TxFlush(void)
{
  U1FCR |= UFCR_TX_FIFO_RESET;          // clear the TX fifo
}


/* Returns: character on success, -1 if no character is available */
int uart0Getch(void)
{
	if (U0LSR & ULSR_RDR)                 // check if character is available
		return U0RBR;                     // return character

	return -1;
}

int uart1Getch(void)
{
	if (U1LSR & ULSR_RDR)                 // check if character is available
		return U1RBR;                     // return character

	return -1;
}

/* Returns: character on success, waits */
int uart0GetchW(void)
{
		while ( !(U0LSR & ULSR_RDR) ); // wait for character
		return U0RBR;                // return character
}

int uart1GetchW(void)
{
	while ( !(U1LSR & ULSR_RDR) ); // wait for character 
	return U1RBR;                // return character
	//if (U1LSR & ULSR_RDR) 
		//	return U1RBR;                // return character
		//else return 0;
}

