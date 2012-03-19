/*
 * intel.h
 *
 *  Created on: Mar 18, 2012
 *      Author: daniel
 */

#ifndef INTEL_H_
#define INTEL_H_

#include <stdint.h>
#include <stdbool.h>

#define ERROR_ERASEBLOCKLOCKED 1
#define ERROR_VPP 2
#define ERROR_PROGERROR 3

bool intel_eraseblock(uint32_t blockaddress);
bool intel_unlockblock(uint32_t blockaddress);
bool intel_lockblock(uint32_t blockaddress);
bool intel_lockdownblock(uint32_t blockaddress);
bool intel_writebyte(uint32_t address, uint8_t byte);
bool intel_writeword(uint32_t address, uint16_t word);
int intel_writeblockasbytes(uint32_t startaddress, int len, uint8_t* data);
int intel_writeblockaswords(uint32_t startaddress, int len, uint8_t* data);

typedef enum {
	unlocked, locked, lockeddown, dunno
} blocklockstatus_t;

typedef struct {
	uint64_t user;
	uint64_t factory;
} intelprotectionregister_t;

#endif /* INTEL_H_ */
