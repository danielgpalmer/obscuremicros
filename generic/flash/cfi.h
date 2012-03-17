/*
 * cfi.h
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#ifndef CFI_H_
#define CFI_H_

#include <stdint.h>

#define READARRAY 					0xFF
#define QUERYADDRESS 				0x55
#define QUERYDATA 					0x98

#define ADDRESS_IDSTRING			0x10
#define ADDRESS_SYSTEMINTERFACE		0x1b
#define ADDRESS_GEOMETRY 			0x27

typedef struct {
	char querystring[4];
	uint16_t prialgid;
	uint16_t prialgaddress;
	uint16_t altalgid;
	uint16_t altalgaddress;
} cfiid_t;

typedef struct {
	float logicmin;
	float logicmax;
	float progmin;
	float progmax;

} cfisysint_t;

typedef struct {
	int bytes;
	uint16_t id;

} cfigeometry_t;

cfiid_t* cfi_getid();
cfisysint_t* cfi_getsysteminterface();
cfigeometry_t* cfi_getgeometry();

#endif /* CFI_H_ */
