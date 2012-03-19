/*
 * common.h
 *
 *  Created on: Mar 19, 2012
 *      Author: daniel
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

#define JEDECIDMODE 0x90
#define READARRAY 0xFF

uint8_t cfi_readbyte(uint8_t address);
uint16_t cfi_readword(uint8_t address);
void cfi_writebyte(uint8_t address, uint8_t data);

#endif /* COMMON_H_ */
