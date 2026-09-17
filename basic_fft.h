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

#ifdef __ADSPSHARCFX__

/* SHARC FX target: use the ADI SHARC FX DSP library complex FFT (cfft/cifft)
 * with the static radix-4 twiddle tables shipped in the toolchain, instead
 * of kiss_fft. No per-instance FFT state is required since the twiddle
 * tables and FFT sizes are compile-time constants. */
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

#else /* !__ADSPSHARCFX__ */

#include "kiss_fft.h"
#include "basic_fft_consts.h"

typedef struct {
    kiss_fft_cfg fftCfg[FFT_MAX_RECORDS];
    kiss_fft_cfg ifftCfg[FFT_MAX_RECORDS];
} IrFftCfg_t;

static void fftCfgInit(IrFftCfg_t* data) {

#if FFT_MAX_RECORDS >= 1
    data->fftCfg[0]  = (kiss_fft_cfg)&fftCfg_0;
    data->ifftCfg[0] = (kiss_fft_cfg)&ifftCfg_0;
#endif
#if FFT_MAX_RECORDS >= 2
    data->fftCfg[1]  = (kiss_fft_cfg)&fftCfg_1;
    data->ifftCfg[1] = (kiss_fft_cfg)&ifftCfg_1;
#endif
#if FFT_MAX_RECORDS >= 3
    data->fftCfg[2]  = (kiss_fft_cfg)&fftCfg_2;
    data->ifftCfg[2] = (kiss_fft_cfg)&ifftCfg_2;
#endif
#if FFT_MAX_RECORDS >= 4
    data->fftCfg[3]  = (kiss_fft_cfg)&fftCfg_3;
    data->ifftCfg[3] = (kiss_fft_cfg)&ifftCfg_3;
#endif  
#if FFT_MAX_RECORDS >= 5
    data->fftCfg[4]  = (kiss_fft_cfg)&fftCfg_4;
    data->ifftCfg[4] = (kiss_fft_cfg)&ifftCfg_4;
#endif
#if FFT_MAX_RECORDS >= 6
    data->fftCfg[5]  = (kiss_fft_cfg)&fftCfg_5;
    data->ifftCfg[5] = (kiss_fft_cfg)&ifftCfg_5;
#endif
#if FFT_MAX_RECORDS >= 7
    data->fftCfg[6]  = (kiss_fft_cfg)&fftCfg_6;
    data->ifftCfg[6] = (kiss_fft_cfg)&ifftCfg_6;
#endif
}

static void IrFftForward(IrFftCfg_t* fft, int cfgIdx, const float_t* in, float_t* out) {
    kiss_fft(fft->fftCfg[cfgIdx], (kiss_fft_cpx*)in, (kiss_fft_cpx*)out);
}

static void IrFftInverse(IrFftCfg_t* fft, int cfgIdx, const float_t* in, float_t* out) {
    kiss_fft(fft->ifftCfg[cfgIdx], (kiss_fft_cpx*)in, (kiss_fft_cpx*)out);
}

#endif /* __ADSPSHARCFX__ */

#endif // __BASIC_FFT_H__
