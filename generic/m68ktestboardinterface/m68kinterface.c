/*
 * m68kinterface.c
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#include "m68kinterface.h"
#include "stubs.h"
#include <stdbool.h>

static pinsin_t pinsin;
static pinsout_t pinsout;

void m68kint_reset(bool reset) {

}

void m68kint_halt(bool halt) {

}

void m68kint_interrupt() {

}

void m68kint_busreq() {

}

void m68kint_clock() {
	getpins(&pinsin);
}

interfacestate_t m68kint_getstate() {
}
