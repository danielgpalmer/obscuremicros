/*
 * cfistubs.h
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#ifndef CFISTUBS_H_
#define CFISTUBS_H_

#include <stdint.h>

void flash_write(uint8_t address, uint8_t data);
uint8_t flash_read(uint8_t address);

#endif /* CFISTUBS_H_ */
