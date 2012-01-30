/*
 * timer.c
 *
 *  Created on: Jan 30, 2012
 *      Author: daniel
 */

#include "timer.h"
#include "lpc22xx.h"

void timer_setup(unsigned int which) {

	switch (which) {
		case 0:
			T0TCR = TIMER_CONTROL_ENABLE | TIMER_CONTROL_RESET;
			break;
		case 1:
			T1TCR = TIMER_CONTROL_ENABLE | TIMER_CONTROL_RESET;
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
