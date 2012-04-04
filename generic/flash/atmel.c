/*
 * atmel.c
 *
 *  Created on: Mar 21, 2012
 *      Author: daniel
 */

#ifndef FLASH_DONTWANT_ATMEL

#include "atmel.h"
#include "flashstubs.h"
#include "jedec.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef FLASH_DONTUSE_MALLOC
static jedecid_t jedecid;
#endif

// This is for the at29c256, should work on the at28hc256 etc

// command preamble
#define COMMAND1 	0xAA
#define COMMAND2	0x55

// The two addresses that command bytes are written to
#define ADDRESS1 	0x5555
#define ADDRESS2 	0x2AAA

// Put the chip into identify mode
#define IDENTIFY 	0x90

// Put the chip back into read array mode
#define READARRAY 	0x50

// For writing and protecting a page, or writing to a protected page
#define WPPAGE		0xA0

// For writing to and unlocking a protected page
#define NWPPAGE1	0x80
#define NWPPAGE2	0x20

// For doing a whole chip erase
#define CHIPERASE1 	0x80
#define CHIPERASE2 	0x10

// The bit that toggles on the databus when the chip is busy
#define TOGGLEBIT	0x40

static void atmel_poll() {

	uint8_t last = flash_read_byte(0) & TOGGLEBIT;
	uint8_t this;
	while (1) {
		this = flash_read_byte(0) & TOGGLEBIT;
		if (this == last) {
			break;
		}
		last = this;
	}
}

static void atmel_issuecommandmode(uint8_t command) {
	flash_write_byte(ADDRESS1, COMMAND1);
	flash_write_byte(ADDRESS2, COMMAND2);
	flash_write_byte(ADDRESS1, command);
}

jedecid_t* atmel_identify() {

#ifndef FLASH_DONTUSE_MALLOC
	jedecid_t* id = malloc(sizeof(jedecid_t));
#else
	jedecid_t* id = &jedecid;
#endif
	if (id != NULL) {
		memset(id, 0, sizeof(jedecid_t));
		atmel_issuecommandmode(IDENTIFY);
		id->mfrid = flash_read_byte(0);
		id->deviceid = flash_read_byte(1);
		atmel_issuecommandmode(READARRAY);
	}

	return id;
}

void atmel_writepage(bool unlock, bool protect, uint32_t pageaddress, uint8_t* data) {

	if (unlock) {
		atmel_issuecommandmode(NWPPAGE1);
		atmel_issuecommandmode(NWPPAGE2);
	}
	else if (protect) {
		atmel_issuecommandmode(WPPAGE);
	}

	for (uint8_t i = 0; i < 64; i++) {
		flash_write_byte(pageaddress + i, *data++);
	}

	atmel_poll();

}

void atmel_chiperase() {
	atmel_issuecommandmode(CHIPERASE1);
	atmel_issuecommandmode(CHIPERASE2);
}

#endif
