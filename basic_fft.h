#ifndef __BASIC_FFT_H__
#define __BASIC_FFT_H__

#include "basic_types.h"

#ifndef FFT_FRAME_MAX_SIZE
#error "Please define FFT_FRAME_MAX_SIZE before including basic_fft.h"
#endif

#if FFT_FRAME_MAX_SIZE == 64
#define FFT_MAX_RECORDS 1
#elif FFT_FRAME_MAX_SIZE == 128
#define FFT_MAX_RECORDS 2
#elif FFT_FRAME_MAX_SIZE == 256
#define FFT_MAX_RECORDS 3
#elif FFT_FRAME_MAX_SIZE == 512
#define FFT_MAX_RECORDS 4
#elif FFT_FRAME_MAX_SIZE == 1024
#define FFT_MAX_RECORDS 5
#elif FFT_FRAME_MAX_SIZE == 2048
#define FFT_MAX_RECORDS 6
#elif FFT_FRAME_MAX_SIZE == 4096
#define FFT_MAX_RECORDS 7
#endif

/*
 * FFT sizes used by the partitioned convolution, indexed by cfgIdx (0..6):
 * 128, 256, 512, 1024, 2048, 4096, 8192.
 */

#define BASIC_FFT_BACKEND_SHARC  1
#define BASIC_FFT_BACKEND_KISSFFT 2
#define BASIC_FFT_BACKEND_RADIX2 3

#ifndef BASIC_FFT_BACKEND
#ifdef __ADSPSHARCFX__
#define BASIC_FFT_BACKEND BASIC_FFT_BACKEND_SHARC
#else
#define BASIC_FFT_BACKEND BASIC_FFT_BACKEND_RADIX2
#endif
#endif

#if BASIC_FFT_BACKEND == BASIC_FFT_BACKEND_SHARC

/* Use the ADI SHARC FX FFT and its static radix-4 twiddle tables. */
#include <dsp/fft.h>
#include <dsp/adi_fft.h>

typedef struct {
    int unused;
} IrFftCfg_t;

static void fftCfgInit(IrFftCfg_t* data) {
    (void)data;
}

static const int irFftSize[7] = { 128, 256, 512, 1024, 2048, 4096, 8192 };

static const complex_float* const irFftTwiddle[7] = {
    twiddles_128_r4,
    twiddles_256_r4,
    twiddles_512_r4,
    twiddles_1024_r4,
    twiddles_2048_r4,
    twiddles_4096_r4,
    twiddles_8192_r4,
};

static void IrFftForward(IrFftCfg_t* fft, int cfgIdx, const float_t* in, float_t* out) {
    (void)fft;
    cfft((const complex_float*)in, (complex_float*)out, irFftTwiddle[cfgIdx], 1, irFftSize[cfgIdx]);
}

static void IrFftInverse(IrFftCfg_t* fft, int cfgIdx, const float_t* in, float_t* out) {
    (void)fft;
    cifft((const complex_float*)in, (complex_float*)out, irFftTwiddle[cfgIdx], 1, irFftSize[cfgIdx]);
}

#elif BASIC_FFT_BACKEND == BASIC_FFT_BACKEND_KISSFFT

#include "../kissfft/kiss_fft.h"
#include "basic_fft_consts.h"

typedef kiss_fft_cfg IrFftBackendCfg_t;

#define IrFftTransform kiss_fft

#elif BASIC_FFT_BACKEND == BASIC_FFT_BACKEND_RADIX2

#include "../newfft/simple_radix2_fft.h"
#include "basic_fft_consts.h"

typedef SimpleRadix2ConfigPtr IrFftBackendCfg_t;
typedef SimpleRadix2Complex IrFftComplex_t;

#define IrFftTransform simple_radix2_fft

#else
#error "Unsupported BASIC_FFT_BACKEND value"

#endif

#if BASIC_FFT_BACKEND != BASIC_FFT_BACKEND_SHARC

#if BASIC_FFT_BACKEND != BASIC_FFT_BACKEND_SHARC
static const int irFftSize[7] = { 128, 256, 512, 1024, 2048, 4096, 8192 };
#endif

typedef struct {
    IrFftBackendCfg_t fftCfg[FFT_MAX_RECORDS];
    IrFftBackendCfg_t ifftCfg[FFT_MAX_RECORDS];
} IrFftCfg_t;

static void fftCfgInit(IrFftCfg_t* data) {

#if FFT_MAX_RECORDS >= 1
    data->fftCfg[0]  = (IrFftBackendCfg_t)&fftCfg_0;
    data->ifftCfg[0] = (IrFftBackendCfg_t)&ifftCfg_0;
#endif
#if FFT_MAX_RECORDS >= 2
    data->fftCfg[1]  = (IrFftBackendCfg_t)&fftCfg_1;
    data->ifftCfg[1] = (IrFftBackendCfg_t)&ifftCfg_1;
#endif
#if FFT_MAX_RECORDS >= 3
    data->fftCfg[2]  = (IrFftBackendCfg_t)&fftCfg_2;
    data->ifftCfg[2] = (IrFftBackendCfg_t)&ifftCfg_2;
#endif
#if FFT_MAX_RECORDS >= 4
    data->fftCfg[3]  = (IrFftBackendCfg_t)&fftCfg_3;
    data->ifftCfg[3] = (IrFftBackendCfg_t)&ifftCfg_3;
#endif  
#if FFT_MAX_RECORDS >= 5
    data->fftCfg[4]  = (IrFftBackendCfg_t)&fftCfg_4;
    data->ifftCfg[4] = (IrFftBackendCfg_t)&ifftCfg_4;
#endif
#if FFT_MAX_RECORDS >= 6
    data->fftCfg[5]  = (IrFftBackendCfg_t)&fftCfg_5;
    data->ifftCfg[5] = (IrFftBackendCfg_t)&ifftCfg_5;
#endif
#if FFT_MAX_RECORDS >= 7
    data->fftCfg[6]  = (IrFftBackendCfg_t)&fftCfg_6;
    data->ifftCfg[6] = (IrFftBackendCfg_t)&ifftCfg_6;
#endif
}

static void IrFftForward(IrFftCfg_t* fft, int cfgIdx, const float_t* in, float_t* out) {
#if BASIC_FFT_BACKEND == BASIC_FFT_BACKEND_KISSFFT
    IrFftTransform(fft->fftCfg[cfgIdx], (const kiss_fft_cpx*)in, (kiss_fft_cpx*)out);
#else
    IrFftTransform(fft->fftCfg[cfgIdx], (const IrFftComplex_t*)in, (IrFftComplex_t*)out);
#endif
}

static void IrFftInverse(IrFftCfg_t* fft, int cfgIdx, const float_t* in, float_t* out) {
#if BASIC_FFT_BACKEND == BASIC_FFT_BACKEND_KISSFFT
    IrFftTransform(fft->ifftCfg[cfgIdx], (const kiss_fft_cpx*)in, (kiss_fft_cpx*)out);
    for (int k = 0; k < (fft->fftCfg[cfgIdx]->nfft); k++) {
        out[k] = out[2 * k] / (fft->fftCfg[cfgIdx]->nfft);
    }         
#else
    IrFftTransform(fft->ifftCfg[cfgIdx], (const IrFftComplex_t*)in, (IrFftComplex_t*)out);
    for (int k = 0; k < (fft->fftCfg[cfgIdx]->nfft); k++) {
        out[k] = out[2 * k];
    }      
#endif
}

#undef IrFftTransform

#endif /* BASIC_FFT_BACKEND != BASIC_FFT_BACKEND_SHARC */

#endif // __BASIC_FFT_H__
