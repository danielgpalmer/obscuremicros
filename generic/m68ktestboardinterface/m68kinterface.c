/*
 * m68kinterface.c
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#include "m68kinterface.h"
#include "stubs.h"
#include <stdbool.h>

static interfacestate_t state = IDLE;
static pinsin_t pinsin;
static pinsout_t pinsout;

static void pushpins() {
	setpins(&pinsout);
}

static void pullpins() {
	getpins(&pinsin);
}

void m68kint_reset(bool reset) {

}

void m68kint_halt(bool halt) {

}

void m68kint_interrupt() {

}

void m68kint_busreq() {

}

void m68kint_clock() {
	switch (state) {
		case IDLE:
			pullpins();
			if (!pinsin.addressstrobe) {
				pinsout.dtack = false;
				pushpins();
				getaddress();
				if (pinsin.readnotwrite) {
					state = READ;
				}
				else {
					state = WRITE;
				}
			}
			break;
		case READ:
			getdata();
			state = ENDTRANSCTION;
			break;
		case WRITE:
			setdata(0xff);
			state = ENDTRANSCTION;
			break;
		case ENDTRANSCTION:
			pinsout.dtack = true;
			pushpins();
			state = IDLE;
			break;
	}
}

interfacestate_t m68kint_getstate() {
}
