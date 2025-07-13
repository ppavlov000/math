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

#ifndef __BASIC_FIXPOINT_VECTOR_MATH_H__
#define __BASIC_FIXPOINT_VECTOR_MATH_H__

#include "basic_types.h"

FORCE_INLINE intg2_t FracToIntgVectorMove(frac2_t in)
{
	intg2_t res;

	res.a = (intg_t)in.a;
	res.b = (intg_t)in.b;

	return res;
}

FORCE_INLINE frac2_t IntgToFracVectorMove(intg2_t in)
{
	frac2_t res;

	res.a = (frac_t)in.a;
	res.b = (frac_t)in.b;

	return res;
}

FORCE_INLINE frac2_t IntgToFracVectorConvert(intg2_t in)
{
	frac2_t res;

	res.a = in.a;
	res.b = in.b;

	return res;
}

FORCE_INLINE intg2_t FracToIntgVectorConvert(frac2_t in)
{
	intg2_t res;

	res.a = in.a;
	res.b = in.b;

	return res;
}

INLINE intg2_t IntgVecMake(intg_t a, intg_t b)
{
	intg2_t ret;

	ret.a = a;
	ret.b = b;

	return ret;
}

#endif //  __BASIC_FIXPOINT_VECTOR_MATH_H__
