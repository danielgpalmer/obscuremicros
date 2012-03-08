/*
 * bk2421io.h
 *
 *  Created on: Feb 22, 2012
 *      Author: daniel
 */

#ifndef BK2421IO_H_
#define BK2421IO_H_

#include <stdint.h>
#include <stdbool.h>

uint8_t bk2421_spi_writebyte();
void bk2421_set_ce(bool high);

#endif /* BK2421IO_H_ */
