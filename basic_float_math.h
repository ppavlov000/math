#ifndef __BASIC_FLOAT_MATH_H__
#define __BASIC_FLOAT_MATH_H__

#include "basic_types.h"

#define LIN_TO_DB(x)   (20.f * log10f((x) + COMP_LOG_FLOOR))
#define DB_TO_LIN(x)   powf(10.f, (x) * 0.05f)

INLINE void Deinterleave(float2_t* in, float_t* out0, float_t* out1, intg_t numSamples) {
   for (intg_t i = 0; i < numSamples; i++) {
        out0[i] = in[i].a;
        out1[i] = in[i].b;
   }
}

INLINE void Interleave(float_t* in0, float_t* in1, float2_t* out, intg_t numSamples) {
    for (intg_t i = 0; i < numSamples; i++) {
        out[i].a = in0[i];
        out[i].b = in1[i];
    }
}

#endif // __BASIC_FLOAT_MATH_H__
