/*
 * intel.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#ifndef FLASH_DONTWANT_INTEL

#include "common.h"
#include "intel.h"
#include "flashstubs.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>

#define STATUS_WRITESTATEMACHINESTATUS 0x80
#define STATUS_ERASESUSPENDSTATUS 0x40
#define STATUS_ERASESTATUS 0x20
#define STATUS_PROGRAMESTATUS 0x10
#define STATUS_VPPSTATUS 0x08
#define STATUS_PROGRAMSUSPENDSTATUS 0x04
#define STATUS_BLOCKLOCKSTATUS 0x02

#define MODE_READARRAY 				READARRAY
#define MODE_READSTATUSREGISTER 	0x70
#define MODE_CLEARSTATUS			0x50
#define MODE_BLOCKERASE				0x20
#define MODE_BLOCKERASE_CONFIRM		0xD0
#define MODE_BLOCKLOCKCHANGESETUP	0x60
#define MODE_BLOCKLOCKBLOCK			0x01
#define MODE_BLOCKUNLOCK			MODE_BLOCKERASE_CONFIRM
#define MODE_BLOCKLOCKDOWN			0x2F
#define MODE_ID						JEDECIDMODE
#define MODE_PROGSETUP				0x40
#define MODE_ALTPROGSETUP			0x10

#define PRLOCKREGISTER				0x80
#define PRREGISTER					0x81
#define PRLEN						8
#define PRSPLIT						4

static uint8_t intel_readstatusregister() {
	flash_write_byte(0, MODE_READSTATUSREGISTER);
	return flash_read_word(0);
}

static uint8_t intel_waitforwsm() {
	uint8_t sr;
	while (!((sr = intel_readstatusregister()) & STATUS_WRITESTATEMACHINESTATUS)) {
	}
	flash_write_byte(0, MODE_CLEARSTATUS);
	return sr;
}

static bool intel_checkeraseerror(uint8_t sr) {
	if (sr & STATUS_ERASESTATUS) {

		if (sr & STATUS_BLOCKLOCKSTATUS) {
			errno = ERROR_ERASEBLOCKLOCKED;
		}
		else {
			errno = 0;
		}

		return false;
	}
	return true;
}

bool intel_eraseblock(uint32_t blockaddress) {
	flash_write_byte(blockaddress, MODE_BLOCKERASE);
	flash_write_byte(blockaddress, MODE_BLOCKERASE_CONFIRM);
	uint8_t sr = intel_waitforwsm();
	flash_write_byte(0, MODE_READARRAY);
	return intel_checkeraseerror(sr);
}

bool intel_unlockblock(uint32_t blockaddress) {
	flash_write_byte(blockaddress, MODE_BLOCKLOCKCHANGESETUP);
	flash_write_byte(blockaddress, MODE_BLOCKUNLOCK);
	uint8_t sr = intel_waitforwsm();
	flash_write_byte(0, MODE_READARRAY);
	return true;
}

bool intel_lockblock(uint32_t blockaddress) {
	flash_write_byte(blockaddress, MODE_BLOCKLOCKCHANGESETUP);
	flash_write_byte(blockaddress, MODE_BLOCKLOCKBLOCK);
	uint8_t sr = intel_waitforwsm();
	flash_write_byte(0, MODE_READARRAY);
	return true;
}

bool intel_lockdownblock(uint32_t blockaddress) {
	flash_write_byte(blockaddress, MODE_BLOCKLOCKCHANGESETUP);
	flash_write_byte(blockaddress, MODE_BLOCKLOCKDOWN);
	uint8_t sr = intel_waitforwsm();
	flash_write_byte(0, MODE_READARRAY);
	return true;
}

blocklockstatus_t intel_getlockstatus(uint32_t blockaddress) {
	flash_write_byte(0, MODE_ID);
	uint8_t blockstatus = (flash_read_byte(blockaddress + 4) & 0x03);
	flash_write_byte(0, MODE_READARRAY);
	switch (blockstatus) {
		case 0:
			return unlocked;
		case 1:
			return locked;
		case 3:
			return lockeddown;
		default:
			return dunno;
	}
}

static bool intel_checkwriteerror(uint8_t sr) {
	errno = 0;
	if (sr & STATUS_VPPSTATUS) {
		errno = ERROR_VPP;
	}
	else if (sr & STATUS_PROGRAMESTATUS) {
		errno = ERROR_PROGERROR;
	}
	else if (sr & STATUS_BLOCKLOCKSTATUS) {
		errno = ERROR_ERASEBLOCKLOCKED;
	}
	else {
		return true;
	}
	return false;
}

static uint8_t writebyte(uint32_t address, uint8_t byte) {
	flash_write_byte(address, MODE_PROGSETUP);
	flash_write_byte(address, byte);
	return intel_waitforwsm();
}

bool intel_writebyte(uint32_t address, uint8_t byte) {
	uint8_t sr = writebyte(address, byte);
	flash_write_byte(0, MODE_READARRAY);
	return intel_checkwriteerror(sr);
}

static uint8_t writeword(uint32_t address, uint16_t word) {
	flash_write_byte(address, MODE_PROGSETUP);
	flash_write_word(address, word);
	return intel_waitforwsm();
}

bool intel_writeword(uint32_t address, uint16_t word) {
	uint8_t sr = writeword(address, word);
	flash_write_byte(0, MODE_READARRAY);
	return intel_checkwriteerror(sr);
}

int intel_writeblockasbytes(uint32_t startaddress, int len, uint8_t* data) {

	int i;
	for (i = 0; i < len; i++) {
		uint8_t sr = writebyte(startaddress + i, data);
		if (intel_checkwriteerror(sr)) {
			break;
		}
	}
	flash_write_byte(0, MODE_READARRAY);
	return i;
}

int intel_writeblockaswords(uint32_t startaddress, int len, uint8_t* data) {

	if (len % 2 != 0) {
		return -1;
	}

	int i;
	for (i = 0; i < len; i + 2) {
		uint8_t sr = writeword(startaddress + i, data);
		if (intel_checkwriteerror(sr)) {
			break;
		}
	}
	flash_write_byte(0, MODE_READARRAY);
	return i;

	return 0;
}

bool intel_getprotectionuserlock() {
	cfi_writebyte(0, MODE_ID);
	uint16_t prlockreg = cfi_readword(PRLOCKREGISTER);
	cfi_writebyte(0, MODE_READARRAY);
	return false;
}

intelprotectionregister_t* intel_getprotectionregister() {
	intelprotectionregister_t* pr = malloc(sizeof(intelprotectionregister_t));
	if (pr != NULL) {
		memset(pr, 0, sizeof(intelprotectionregister_t));
		cfi_writebyte(0, MODE_ID);
		for (int i = 0; i < PRLEN; i++) {
			uint16_t word = cfi_readword(PRREGISTER + i);
			printf("word %d is 0x%04x\n", i, word);
			if (i < PRSPLIT) {
				pr->factory |= (word << (16 * i));
				printf("f\n", i, word);
			}
			else {
				pr->user |= (word << (16 * (i - PRSPLIT)));
				printf("u\n", i, word);
			}
		}
		cfi_writebyte(0, MODE_READARRAY);
	}
	return pr;
}

#endif
