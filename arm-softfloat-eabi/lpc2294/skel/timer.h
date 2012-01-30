/*
 * timer.h
 *
 *  Created on: Jan 30, 2012
 *      Author: daniel
 */

#ifndef TIMER_H_
#define TIMER_H_

#define TIMER_ENABLE 1
#define TIMER_RESET (1 << 1)

// T?IR

#define TIMER_INTERRUPT_MR0 1
#define TIMER_INTERRUPT_MR1 (1 << 1)
#define TIMER_INTERRUPT_MR2 (1 << 2)
#define TIMER_INTERRUPT_MR3 (1 << 3)
#define TIMER_INTERRUPT_CR0 (1 << 4)
#define TIMER_INTERRUPT_CR1 (1 << 5)
#define TIMER_INTERRUPT_CR2 (1 << 6)
#define TIMER_INTERRUPT_CR3 (1 << 7)

// T?TCR

#define TIMER_MODE_EVERY 00
#define TIMER_MODE_CAPRISINGEDGE 01
#define TIMER_MODE_CAPFALLINGEDGE 10
#define TIMER_MODE_CAPBOTHEDGES 11

#define TIMER_COUNTINPUTSELECT_CAP0 (00 << 2)
#define TIMER_COUNTINPUTSELECT_CAP1 (01 << 2)
#define TIMER_COUNTINPUTSELECT_CAP2 (10 << 2)
#define TIMER_COUNTINPUTSELECT_CAP3 (11 << 2)

// T?MCR

#define TIMER_MATCHCONTROL_MR0I 1
#define TIMER_MATCHCONTROL_MR0R (1 << 1)
#define TIMER_MATCHCONTROL_MR0S (1 << 2)
#define TIMER_MATCHCONTROL_MR1I (1 << 3)
#define TIMER_MATCHCONTROL_MR1R (1 << 4)
#define TIMER_MATCHCONTROL_MR1S (1 << 5)
#define TIMER_MATCHCONTROL_MR2I (1 << 6)
#define TIMER_MATCHCONTROL_MR2R (1 << 7)
#define TIMER_MATCHCONTROL_MR2S (1 << 8)
#define TIMER_MATCHCONTROL_MR3I (1 << 9)
#define TIMER_MATCHCONTROL_MR3R (1 << 10)
#define TIMER_MATCHCONTROL_MR4S (1 << 11)

void timer_setup(unsigned int which);
void timer_start(unsigned int which);

#endif /* TIMER_H_ */
