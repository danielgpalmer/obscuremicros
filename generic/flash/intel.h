/*
 * intel.h
 *
 *  Created on: Mar 18, 2012
 *      Author: daniel
 */

#ifndef INTEL_H_
#define INTEL_H_

#include <stdint.h>

#define STATUS_WRITESTATEMACHINEBUSY 0x80
#define STATUS_ERASESUSPENDED 0x40
#define STATUS_ERASESTATUS 0x20
#define STATUS_PROGRAMESTATUS 0x10
#define STATUS_VPP 0x08

uint8_t intel_readstatusregister();

#endif /* INTEL_H_ */
