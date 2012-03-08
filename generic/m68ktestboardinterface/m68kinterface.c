/*
 * m68kinterface.c
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#include "m68kinterface.h"
#include "stubs.h"
#include <stdbool.h>
#include <stdio.h>

static interfacestate_t state = IDLE;
static transactionrange_t range = WORD;
static pinsin_t pinsin;
static pinsout_t pinsout;
static uint32_t address;
static

static int cyclecounter = 0;

static void pushpins() {
	setpins(&pinsout);
}

static void pullpins() {
	getpins(&pinsin);
}

void m68kint_reset() {
	cyclecounter = 0;
	state = RESET;
}

void m68kint_interrupt() {

}

void m68kint_busreq() {
	pinsout.busreq = false;
	pushpins();
}

void m68kint_clock() {
	switch (state) {
		case RESET:
			cyclecounter++;
			if (cyclecounter > 100) {
				pinsout.reset = true;
				pinsout.halt = true;
				state = IDLE;
			}
			else {
				pinsout.reset = false;
				pinsout.halt = false;
			}
			pushpins();
			break;

		case IDLE:
			pullpins();
			if (!pinsin.addressstrobe) {

				if (!pinsin.upperdatastrobe && !pinsin.lowerdatastrobe) {
					range = WORD;
				}
				else if (!pinsin.upperdatastrobe) {
					range = UPPERBYTE;
				}
				else if (!pinsin.lowerdatastrobe) {
					range = LOWERBYTE;
				}
				else {
					printf("data strobes are wrong! ABORT!! woo woo\n");
					return;
				}

				pinsout.dtack = false;
				pushpins();
				address = getaddress();
				if (pinsin.readnotwrite) {
					printf("m68kint -- read transaction started\n");
					state = READ;
				}
				else {
					printf("m68kint -- write transaction started\n");
					state = WRITE;
				}
			}
			break;
		case READ:
			switch (range) {
				case WORD:
					setdata(readword(address));
					break;
			}
			state = ENDTRANSCTION;
			break;
		case WRITE:
			switch (range) {
				case WORD:
					writeword(address, getdata());
					break;
			}
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
	return state;
}
