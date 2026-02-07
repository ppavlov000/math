/*
Copyright (c) 2017-2025 Petro Pavlov ppavlov000@gmail.com

Basic math software is provided under:
  SPDX-License-Identifier: BSD-3-Clause

All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*/

#ifndef __BASIC_TYPES_H__
#define __BASIC_TYPES_H__

#include <stdint.h>
#include <stddef.h>
#include <math.h>

#define INLINE          static __inline
#define FORCE_INLINE    __inline
#define PREVENT_INLINE  static
#define     __restrict

#define ABS(a)   ((a)<0?-(a):(a))
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#define FRACTIONAL_BITS     (32)
#define INT_BASE			(5) 

typedef     int8_t      frac8_t;
typedef     int16_t     frac16_t;
typedef     int32_t     frac_t;
typedef     int64_t     acc_t;
typedef     int32_t     intg_t;
typedef     uint32_t    uintg_t;
typedef     double		double_t;
typedef     float		float_t;


#define DOUBLE_PI (double_t)3.1415926535897932384626433832795
#define ONE32       0x7fffffff
#define MONE32      0x80000000

#define ONE64       0x7fffffffFFFFFFFFLL
#define MONE64      0x8000000000000000LL

#define ONE32_64    0x000000007fffffffLL
#define MONE32_64   0xFFFFFFFF80000000LL

typedef struct 
{
    intg_t a ;
    intg_t b ;
}intg2_t ;

typedef struct 
{
	frac16_t a;
	frac16_t b;
}fract16D2_t;

typedef struct 
{
	frac8_t a;
	frac8_t b;

}fract8D2_t;

typedef struct 
{
	frac_t a;
	frac_t b;
}frac2_t;

typedef struct 
{
	float_t a;
	float_t b;
}float2_t;

typedef struct 
{
	double_t a;
	double_t b;
}double2_t;

typedef struct 
{
    acc_t a ;
    acc_t b ;
}acc2_t;

#endif // __BASIC_TYPES_H__
