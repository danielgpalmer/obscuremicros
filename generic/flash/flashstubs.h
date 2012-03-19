/*
 * cfistubs.h
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#ifndef CFISTUBS_H_
#define CFISTUBS_H_

#include <stdint.h>

void flash_write_byte(uint32_t address, uint8_t data);
void flash_write_word(uint32_t address, uint16_t data);
uint8_t flash_read_byte(uint32_t address);
uint16_t flash_read_word(uint32_t address);

#endif /* CFISTUBS_H_ */
