#ifndef __BASIC_FLOAT_MATH_H__
#define __BASIC_FLOAT_MATH_H__

#include "basic_types.h"

INLINE void deinterleave(float2_t* in, float_t* out0, float_t* out1, intg_t numSamples) {
   for (intg_t i = 0; i < numSamples; i++) {
        out0[i] = in[i].a;
        out1[i] = in[i].b;
   }
}

INLINE void interleave(float_t* in0, float_t* in1, float2_t* out, intg_t numSamples) {
    for (intg_t i = 0; i < numSamples; i++) {
        out[i].a = in0[i];
        out[i].b = in1[i];
    }
}

#endif // __BASIC_FLOAT_MATH_H__
