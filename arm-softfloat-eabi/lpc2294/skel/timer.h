/*
 * timer.h
 *
 *  Created on: Jan 30, 2012
 *      Author: daniel
 */

#ifndef TIMER_H_
#define TIMER_H_

// T?IR -- Interrupt Register

#define TIMER_INTERRUPT_MR0 1
#define TIMER_INTERRUPT_MR1 (1 << 1)
#define TIMER_INTERRUPT_MR2 (1 << 2)
#define TIMER_INTERRUPT_MR3 (1 << 3)
#define TIMER_INTERRUPT_CR0 (1 << 4)
#define TIMER_INTERRUPT_CR1 (1 << 5)
#define TIMER_INTERRUPT_CR2 (1 << 6)
#define TIMER_INTERRUPT_CR3 (1 << 7)

// T?TCR -- Timer Control Register

#define TIMER_CONTROL_ENABLE 1
#define TIMER_CONTROL_RESET (1 << 1)

// T?CTCR -- Count Control Register

#define TIMER_MODE_EVERY 00
#define TIMER_MODE_CAPRISINGEDGE 01
#define TIMER_MODE_CAPFALLINGEDGE 10
#define TIMER_MODE_CAPBOTHEDGES 11

#define TIMER_COUNTINPUTSELECT_CAP0 (00 << 2)
#define TIMER_COUNTINPUTSELECT_CAP1 (01 << 2)
#define TIMER_COUNTINPUTSELECT_CAP2 (10 << 2)
#define TIMER_COUNTINPUTSELECT_CAP3 (11 << 2)

// T?MCR -- Match Control Register

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

// T?CCR -- Capture Control Register

#define TIMER_CAPCONTROL_CAP0RE	1
#define TIMER_CAPCONTROL_CAP0FE	(1 << 1)
#define TIMER_CAPCONTROL_CAP0I	(1 << 2)
#define TIMER_CAPCONTROL_CAP1RE	(1 << 3)
#define TIMER_CAPCONTROL_CAP1FE	(1 << 4)
#define TIMER_CAPCONTROL_CAP1I	(1 << 5)
#define TIMER_CAPCONTROL_CAP2RE	(1 << 6)
#define TIMER_CAPCONTROL_CAP2FE	(1 << 7)
#define TIMER_CAPCONTROL_CAP2I	(1 << 8)
#define TIMER_CAPCONTROL_CAP3RE	(1 << 9)
#define TIMER_CAPCONTROL_CAP3FE	(1 << 10)
#define TIMER_CAPCONTROL_CAP3I	(1 << 11)

// T?EMR -- External Match Register

#define TIMER_EMATCH_EM0 1
#define TIMER_EMATCH_EM1 (1 << 1)
#define TIMER_EMATCH_EM2 (1 << 2)
#define TIMER_EMATCH_EM3 (1 << 3)

void timer_reset(unsigned int which);
void timer_start(unsigned int which);

#endif /* TIMER_H_ */
