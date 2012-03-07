/*
 * m68kinterface.h
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#ifndef M68KINTERFACE_H_
#define M68KINTERFACE_H_

#include <stdbool.h>
#include <stdint.h>


typedef enum {
	IDLE, READ, WRITE, ENDTRANSCTION, RESET
} interfacestate_t;

typedef enum {
	WORD, LOWERBYTE, UPPERBYTE
} transactionrange_t;

typedef struct {
	bool addressstrobe;
	uint8_t function;
	bool readnotwrite;
	bool upperdatastrobe;
	bool lowerdatastrobe;
} pinsin_t;

typedef struct {
	bool dtack;
	bool reset;
	bool halt;
	bool busreq;
	uint8_t ipl;
} pinsout_t;

void m68kint_clock();
void m68kint_reset();
void m68kint_interrupt();
void m68kint_busreq();
interfacestate_t m68kint_getstate();

#endif /* M68KINTERFACE_H_ */
