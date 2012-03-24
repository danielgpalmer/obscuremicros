/*
 * common.c
 *
 *  Created on: Mar 19, 2012
 *      Author: daniel
 */

#include "common.h"
#include "flashstubs.h"
#include <stdint.h>

uint16_t cfi_readword(uint8_t address) {
	return flash_read_word(address * 2);
}

uint8_t cfi_readbyte(uint8_t address) {
	return (cfi_readword(address) & 0xFF);
}

void cfi_writebyte(uint8_t address, uint8_t data) {
	flash_write_word(address * 2, data);
}
