/*
 * ZigBeeDataTypes.h
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#ifndef ZIGBEEDATATYPES_H_
#define ZIGBEEDATATYPES_H_

#include "data_types.h"

//include "common_lmac.h"
//include "cpu.h"

//! unsigned char is mapped to uint8
typedef uint8 uint8_t,BOOL;
//! char is mapped to int8
typedef signed char  int8_t;
//! unsigned short int is mapped as uint16
typedef uint16 uint16_t;
//! short int is mapped as int16
typedef int16 int16_t;
//! unsigned int is mapped as uint32
typedef uint32 uint32_t;
//! int is mapped as uint32
typedef int32 int32_t;
//! unsigned int is mapped as uint32
typedef unsigned long long int uint64_t;
//! unsigned long long int
typedef unsigned long long int uint64;
//! int is mapped as uint32
typedef long long int int64, int64_t;
//! Signed int is mapped as sint32
typedef signed int sint32;

//#define ENTER_CRITICAL_REGION(x)  x = conditional_intr_disable();
//#define LEAVE_CRITICAL_REGION(x)  conditional_intr_enable(x);

#endif /* ZIGBEEDATATYPES_H_ */
