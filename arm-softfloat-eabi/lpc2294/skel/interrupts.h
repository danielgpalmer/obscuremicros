/*
 * interrupts.h
 *
 *  Created on: 2 May 2011
 *      Author: daniel
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#define INTCHAN_UART1 0x80;

#define INTERRUPT_CHANNEL_ENABLE 0x20

#define INTERRUPT_CHANNEL_TIMER0 0x4
#define INTERRUPT_MASK_TIMER0 0x10

unsigned enableFIQ(void);
unsigned enableIRQ(void);
void disableInterrupts(void);
void dumpInterruptRegisters(void);

void interrupts_reset();

#endif /* INTERRUPTS_H_ */
