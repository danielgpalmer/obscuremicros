/*
 * atmel.h
 *
 *  Created on: Mar 21, 2012
 *      Author: daniel
 */

#ifndef ATMEL_H_
#define ATMEL_H_

#include <stdbool.h>
#include <stdint.h>
#include "jedec.h"

jedecid_t* atmel_identify(void);
void atmel_writepage(bool unlock, bool protect, uint32_t pageaddress, uint8_t* data);
void atmel_chiperase(void);

#endif /* ATMEL_H_ */
