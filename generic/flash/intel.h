/*
 * intel.h
 *
 *  Created on: Mar 18, 2012
 *      Author: daniel
 */

#ifndef INTEL_H_
#define INTEL_H_

#include <stdint.h>

#define ERROR_ERASEBLOCKLOCKED 1

#define STATUS_WRITESTATEMACHINESTATUS 0x80
#define STATUS_ERASESUSPENDSTATUS 0x40
#define STATUS_ERASESTATUS 0x20
#define STATUS_PROGRAMESTATUS 0x10
#define STATUS_VPPSTATUS 0x08
#define STATUS_PROGRAMSUSPENDSTATUS 0x04
#define STATUS_BLOCKLOCKSTATUS 0x02

uint8_t intel_readstatusregister();

#endif /* INTEL_H_ */
