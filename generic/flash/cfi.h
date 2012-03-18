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
#define ADDRESS_GEOMETRY_ERASEBINFO	0x2D

#define GEOMETRY_SIZE				(ADDRESS_GEOMETRY)
#define GEOMETRY_FDICD_LOW			(GEOMETRY_SIZE + 1)
#define GEOMETRY_FDICD_HIGH			(GEOMETRY_FDICD_LOW + 1)
#define GEOMETRY_MULTIBYTE_LOW			(GEOMETRY_FDICD_HIGH + 1)
#define GEOMETRY_MUTLIBYTE_HIGH			(GEOMETRY_MULTIBYTE_LOW + 1)
#define GEOMERTY_ERASEREGIONS		(GEOMETRY_MUTLIBYTE_HIGH + 1)

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
	uint32_t blocksize;
	uint16_t numblocks;

} cfieraseblockinfo_t;

typedef struct {
	uint32_t bytes;
	uint16_t flashdeviceinterfacecode;
	uint16_t multibytemax;
	uint8_t eraseblockregions;
	cfieraseblockinfo_t* eraseblockinfo;
} cfigeometry_t;

cfiid_t* cfi_getid();
cfisysint_t* cfi_getsysteminterface();
cfigeometry_t* cfi_getgeometry();
void cfi_freegeometry(cfigeometry_t* geo);

#endif /* CFI_H_ */
