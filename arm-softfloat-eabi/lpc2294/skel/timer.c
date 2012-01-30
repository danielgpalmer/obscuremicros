/*
 * timer.c
 *
 *  Created on: Jan 30, 2012
 *      Author: daniel
 */

#include "timer.h"
#include "lpc22xx.h"

void timer_reset(unsigned int which) {

	switch (which) {
		case 0:
			T0TCR = TIMER_CONTROL_ENABLE | TIMER_CONTROL_RESET;
			T0IR = 0xFF;
			T0MCR = 0;
			T0CCR = 0;
			T0EMR = 0;
			break;
		case 1:
			T1TCR = TIMER_CONTROL_ENABLE | TIMER_CONTROL_RESET;
			T1IR = 0xFF;
			T1MCR = 0;
			T1CCR = 0;
			T1EMR = 0;
			break;
	}

}

void timer_start(unsigned int which) {

	switch (which) {
		case 0:
			T0TCR &= ~TIMER_CONTROL_RESET;
			break;
		case 1:
			T1TCR &= ~TIMER_CONTROL_RESET;
			break;
	}

}
