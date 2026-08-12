#ifndef __BASIC_FLOAT_MATH_H__
#define __BASIC_FLOAT_MATH_H__

#include "basic_types.h"

#define SMOOTHING_ENABLE 0

#define LIN_TO_DB(x)   (20.f * log10f((x) + COMP_LOG_FLOOR))
#define DB_TO_LIN(x)   powf(10.f, (x) * 0.05f)

static void Deinterleave(float2_t* in, float_t* out0, float_t* out1, intg_t numSamples) {
   for (intg_t i = 0; i < numSamples; i++) {
        out0[i] = in[i].a;
        out1[i] = in[i].b;
   }
}

static void Interleave(float_t* in0, float_t* in1, float2_t* out, intg_t numSamples) {
    for (intg_t i = 0; i < numSamples; i++) {
        out[i].a = in0[i];
        out[i].b = in1[i];
    }
}

#if SMOOTHING_ENABLE
static void SmoothValue(float_t* current, float_t* target, float_t step) {
    float_t delta = *target - *current;
    if (fabsf(delta) <= step) {
        *current = *target;
    }
    else {
        if (delta > 0) {
            *current += step;
        }
        else {
            *current -= step;
        }
    }
}
#else
static void SmoothValue(float_t* current, float_t* target, float_t step) {
    *current = *target;
}
#endif

#endif // __BASIC_FLOAT_MATH_H__
