/*
 * stubs.h
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#ifndef STUBS_H_
#define STUBS_H_

#include <stdint.h>
#include "m68kinterface.h"

uint16_t readword(uint32_t address); // should return a word from the backing store
void writeword(uint32_t address, uint16_t data); // should push a word into the backing store
uint32_t getaddress();
uint16_t getdata();
void setdata(uint16_t data);
void getpins(pinsin_t* pinsin);
void setpins(pinsout_t* pinsout);
void setclock(bool clock);

#endif /* STUBS_H_ */
