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

static uint16_t memory[1024];
static bool addressstrobe = true;
static bool read = true;
static bool uds = true;
static bool lds = true;
static bool dtack = true;
static uint8_t function;
static uint32_t address;
static uint16_t dataout, datain;

uint16_t readword(uint32_t address) {
	printf("readword(0x%08x)\n", address);
	return 0xFFFF;
}

void writeword(uint32_t address, uint16_t data) {
	printf("writeword(0x%08x, 0x%04x)\n", address, data);
}

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
	printf("setdata(0x%04x)\n", data);
}

void resetstate() {
	addressstrobe = true;
	read = true;
	uds = true;
	lds = true;
	dtack = true;
	m68kint_clock();
}

bool runtransaction() {
	int cyclecounter = 0;
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
		return false;
	}

	printf("Test seems to have worked\n");

	return true;
}

int main(int argc, char* argv[]) {

	m68kint_reset();
	while (m68kint_getstate() == RESET) {
		m68kint_clock();
		printf("spinning on reset\n");
	}

	resetstate();
	printf("Testing a word read cycle\n");
	addressstrobe = false;
	address = 0xFFFF;
	read = true;
	uds = false;
	lds = false;
	if (!runtransaction()) {
		return 1;
	}

	resetstate();
	printf("Testing a low byte read cycle\n");

	resetstate();
	printf("Testing a high byte read cycle\n");

	resetstate();
	printf("Testing a write cycle\n");
	addressstrobe = false;
	address = 0xFFFF;
	dataout = 0xFFFF;
	read = false;
	uds = false;
	lds = false;
	dataout = 0xFF;
	if (!runtransaction()) {
		return 1;
	}

}
