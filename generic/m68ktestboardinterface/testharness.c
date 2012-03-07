/*
 * testharness.c
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#include "m68kinterface.h"
#include "stubs.h"

#include <stdio.h>
#include <stdint.h>

static bool addressstrobe = true;
static bool read = true;
static bool uds = true;
static bool lds = true;
static bool dtack = true;
static uint8_t function;
static uint32_t address;
static uint16_t dataout, datain;

void getpins(pinsin_t* pinsin) {
	printf("getpins()\n");
	pinsin->addressstrobe = addressstrobe;
	pinsin->readnotwrite = read;
	pinsin->lowerdatastrobe = lds;
	pinsin->upperdatastrobe = uds;
}

void setpins(pinsout_t* pinsout) {
	printf("setpins()\n");
	dtack = pinsout->dtack;
}

uint32_t getaddress() {
	printf("getaddress()\n");
	return address;
}

uint16_t getdata() {
	printf("getdata()\n");
	return dataout;
}

void setdata(uint16_t data) {
	printf("setdata()\n");
}

void resetstate() {
	addressstrobe = true;
	read = true;
	uds = true;
	lds = true;
	dtack = true;
}

int main(int argc, char* argv[]) {

	int cyclecounter = 0;

	printf("Testing a word read cycle\n");
	addressstrobe = false;
	address = 0xFFFF;
	read = true;
	uds = true;
	lds = true;

	// wait for dtack to go low
	while (dtack == true && cyclecounter < 10) {
		m68kint_clock();
		cyclecounter++;
	}

	// wait while its low
	while (dtack == false && cyclecounter < 10) {
		m68kint_clock();
		cyclecounter++;
	}

	addressstrobe = true;

	if (cyclecounter == 10) {
		printf("Failed\n");
		return 1;
	}

	printf("Testing a low byte read cycle\n");

	printf("Testing a high byte read cycle\n");

	printf("Testing a write cycle\n");

}
