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

#ifndef __BASIC_FIXPOINT_MATH_H__
#define __BASIC_FIXPOINT_MATH_H__

#include "basic_types.h"

FORCE_INLINE intg_t FracToIntgMove(frac_t in)
{
	return (intg_t)in;
}

FORCE_INLINE acc_t FracToAccMove(frac_t in)
{
	acc_t acc = in;
	return acc << 32;
}

FORCE_INLINE frac_t IntgToFracMove(intg_t in)
{
	return (frac_t)in;
}

FORCE_INLINE intg_t FracToIntgConvert(frac_t in)
{
	return (intg_t)in;
}

FORCE_INLINE frac_t IntgToFracConvert(intg_t in)
{
	return (frac_t)in;
}

INLINE frac_t DoubleToFrac(double_t x)
{
	frac_t res = x * ONE32;

	return res;
}

FORCE_INLINE double_t FracToDouble(frac_t x)
{
	double_t res = (double_t)x / ONE32;

	return res;
}

INLINE frac_t Saturate(acc_t acc)
{
    acc_t res = (acc >= 0)? MIN(acc, 0x000000007FFFffffLL) : MAX(acc, 0xFFFFffff80000000LL);

    return (frac_t)res;
}

INLINE intg_t FracNorm(frac_t x)
{
	intg_t ex = 0;
	if (x < 0)
		x = -x;                 
	if (MONE32 == x)
		x = ONE32;

	if (x != 0)
	{
		for (ex = 0; x > 0; x <<= 1){ 
			ex++;
		}
		ex--;
	}

	return ex;
}

INLINE frac_t FracAbs(frac_t x)
{
	frac_t res = x;

	if (x < 0) res = -x;

	return res;
}

INLINE frac_t FracADDS(frac_t x, frac_t y)
{
    acc_t acc;
    frac_t res;

    acc = (acc_t)x + (acc_t)y;
    res = Saturate(acc);

    return res;
}

INLINE frac_t FracADD(frac_t x, frac_t y)
{
    return (x + y);
}

INLINE frac_t FracSUBS(frac_t x, frac_t y)
{
    acc_t acc;
    frac_t res;

    acc = (acc_t)x - (acc_t)y;
    res = Saturate(acc);

    return res;
}

INLINE frac_t FracSUB(frac_t x, frac_t y)
{
    return (x - y);
}

INLINE acc_t AccSetLow(frac_t x)
{
	acc_t acc;

	acc = (uint32_t)x;
	return acc;
}

INLINE frac_t AccGetLow(acc_t acc)
{
	frac_t res;

	res = (frac_t)acc;

	return res;
}

INLINE acc_t AccSetSaturation(acc_t acc)
{
    if(acc >= 0)
        acc = (acc_t) ONE64 ;
    else
        acc = (acc_t) MONE64;

    return acc;
}


INLINE acc_t AccLeftShift(acc_t acc, intg_t n)
{
    acc_t tmp;
    tmp = acc << n;
    if((tmp >> n) != acc){
        acc = AccSetSaturation(acc);   
    }
    else{
        acc = tmp;
    }

    return acc;
}

INLINE acc_t AccLeftShift1Bit(acc_t acc)
{
    return AccLeftShift(acc, 1);
}

INLINE acc_t AccRightShift1Bit(acc_t acc)
{
    return acc >> 1;
}

INLINE acc_t AccRightShift(acc_t acc, intg_t n)
{
    return acc >> n;
}

INLINE acc_t AccMUL(frac_t x, frac_t y)
{
    acc_t acc;

    acc = (acc_t)x * (acc_t)y;
    acc = AccLeftShift1Bit(acc);        
    return acc ;
}

INLINE acc_t AccMAC(acc_t acc, frac_t x, frac_t y)
{
    acc_t tmpAcc;
    acc_t mulRes;

    tmpAcc = acc;                   
    mulRes = AccMUL(x, y);   
    acc += mulRes;                  
    if( ((tmpAcc >= 0) && (mulRes >= 0) && (acc < 0)) ||   
        ((tmpAcc < 0) && (mulRes < 0) && (acc >= 0)) )      
    {
        acc = AccSetSaturation(tmpAcc);             
    }

    return acc ;
}

INLINE acc_t AccMSUB(acc_t acc, frac_t x, frac_t y)
{
    acc_t tmpAcc;
    acc_t mulRes;

    tmpAcc = acc;                   
    mulRes = AccMUL(x, y);   
    acc -= mulRes;                  
    if( ((tmpAcc >= 0) && (mulRes < 0) && (acc < 0)) ||    
        ((tmpAcc < 0) && (mulRes >= 0) && (acc >= 0)) )    
    {
        acc = AccSetSaturation(tmpAcc);            
    }

	return acc ;
}

INLINE frac_t AccGetLow( acc_t acc )
{
   frac_t res ;
   res = (frac_t)acc;

   return res;
}

INLINE frac_t AccGetHigh( acc_t acc )
{
    frac_t res ;
    res = (frac_t)(acc >> 32);

	return res ;
}

INLINE frac_t AccGetHighRound(acc_t acc)
{
    acc_t tmp;
    frac_t res;
  
    tmp = acc + (acc_t)0x80000000;
    if((acc > 0) && (tmp < 0)){
        tmp = (acc_t) ONE64 ;	
    }
    acc = tmp;
    res = (frac_t)(acc >> 32);

    return res;
}

INLINE acc_t AccFracMUL(acc_t x, frac_t y)
{
	acc_t acc;

	acc = (acc_t)AccGetHigh(x) * (acc_t)y;
	acc = AccLeftShift1Bit(acc);

	return acc;
}

INLINE frac_t FracMULRound(frac_t x, frac_t y)
{
    acc_t acc;
    frac_t res;

    acc = AccMUL(x, y);
    res = AccGetHighRound(acc);

    return res;
}

INLINE frac_t FracMul(frac_t x, frac_t y)
{
	acc_t acc;
	frac_t res;

	acc = AccMUL(x, y);
	res = AccGetHigh(acc);

	return res;
}

INLINE frac_t FracLeftShift( frac_t x, intg_t n )
{
    frac_t d ;
    acc_t ret = x;

	ret <<= n;
    d = Saturate(ret);

	return d ;
}

INLINE frac_t FracLeftShift1Bit( frac_t x )
{
    frac_t d ;
    acc_t ret = x;

	ret <<= 1;
	d = Saturate(ret);

	return d ;
}

INLINE frac_t FracRightShift( frac_t x, intg_t n )
{
    frac_t d;
	d = x >> n;

	return d;
}

INLINE frac_t FracRightShift1Bit( frac_t x )
{
    frac_t d;
    d = x >> 1;

	return d;
}

INLINE frac_t FractZero()
{
    return 0;
}


INLINE acc_t AccFracAdds(acc_t acc, frac_t y)
{
	acc_t tmpAcc;

	tmpAcc = acc;
	acc += y;

	if (((tmpAcc >= 0) && (y >= 0) && (acc < 0)) ||
		((tmpAcc < 0) && (y < 0) && (acc >= 0)))
	{
		acc = AccSetSaturation(tmpAcc);
	}

	return acc;
}

INLINE acc_t FracIntMul(frac_t f, intg_t y)
{
	acc_t tmpAcc = f;
	acc_t acc = f;
	acc = acc * y;

	if (((tmpAcc >= 0) && (y >= 0) && (acc < 0)) ||
		((tmpAcc < 0) && (y < 0) && (acc >= 0)))
	{
		acc = AccSetSaturation(tmpAcc);
	}

	return acc;
}

INLINE frac_t FractionalLeftShiftNoSaturation(frac_t x, intg_t n)
{
	frac_t d;
	d = (acc_t)x << n;

	return d;
}


#endif //  __BASIC_FIXPOINT_MATH_H__
