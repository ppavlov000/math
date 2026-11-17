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


#if defined __ADSPSHARCFX__
// Analog devices SHARC FX 21835 platform
#include <dsp/vector.h>
#include <xtensa/tie/adi_sharc_fx0.h>
#define ar_sqrtf(x)    ADI_SQRT_F32(x)
#define ar_vec_mul(x, y, z, N) vecvmltf (x, y, z, N);

#else
// PC platform
#define ar_sqrtf(x)    sqrtf(x)

// void ar_vec_mul (const float_t *x, const float_t *y, float_t *z, int N)
// {
//     for (intg_t idx = 0; idx < N; idx++){
//         z[idx] = x[idx] * y[idx];
//     }
// }

// void ar_vec_complex_power(const float* re, const float*  im, float* pw, int n)
// {
//     for (int i = 0; i < n; i++){
//         pw[i] = re[i] * re[i] + im[i] * im[i];
//     }
// }

#endif

#endif // __BASIC_FLOAT_MATH_H__
