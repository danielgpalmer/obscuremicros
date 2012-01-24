/*
 * interrupts.h
 *
 *  Created on: 2 May 2011
 *      Author: daniel
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#define INTCHAN_UART1 0x80;

unsigned enableFIQ(void);
unsigned enableIRQ(void);
void disableInterrupts(void);
void dumpInterruptRegisters(void);

#endif /* INTERRUPTS_H_ */
