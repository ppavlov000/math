#ifndef __BASIC_EQ_MATH_H__
#define __BASIC_EQ_MATH_H__

#include "basic_types.h"
#include "basic_fixpoint_vector_math.h"

#define PEQ_COEFS_HEADROOM 0

typedef enum
{
    EQ_TYPE_LPF = 0,
    EQ_TYPE_HPF,
    EQ_TYPE_BP,
    EQ_TYPE_BP0DB,
    EQ_TYPE_NOTCH,
    EQ_TYPE_PEAK,
    EQ_TYPE_LSHELF,
    EQ_TYPE_HSHELF,
    EQ_TYPE_APF,
} BiquadType_t;

typedef struct
{
    int Active;
    BiquadType_t Type;
    float Frequency;
    float Gain;
    float Q;
} BiquadParam_t;

typedef struct
{
    intg_t active;
    float_t a[3];
    float_t b[3];
    intg_t shifts_a;
    intg_t shifts_b;
    float_t shifts_a_mul;
    float_t shifts_b_mul;
} BiquadFloatMonoCoefs_t;

typedef struct
{
    float_t NS;
    float_t x1;
    float_t x2;
} BiquadFloatMonoStates_t;

typedef struct
{
    float2_t NS;
    float2_t x1;
    float2_t x2;
} BiquadFloatStereoStates_t;

typedef struct
{
    intg_t active;
    float2_t a[3];
    float2_t b[3];
    intg2_t shifts_a;
    intg2_t shifts_b;
    float2_t shifts_a_mul;
    float2_t shifts_b_mul;
} BiquadFloatStereoCoefs_t;

static void BiquadMonoFloatProcessingSimple(const float_t *pIn, float_t *pOut, const BiquadFloatMonoCoefs_t *pCoefs, BiquadFloatMonoStates_t *pStates)
{
    float_t acc;
    float_t acc_a = 0;
    float_t acc_b = 0;
    float_t dIn;

    dIn = *pIn;
    acc = pStates[0].NS;
    acc_a = dIn * pCoefs->a[0];
    acc_a = acc_a + pStates[0].x1 * pCoefs->a[1];
    acc_a = acc_a + pStates[0].x2 * pCoefs->a[2];
    acc_a = acc_a * pCoefs->shifts_a_mul;
    acc_b = acc_a * pCoefs->b[0];
    acc_b = acc_b + pStates[0].x1 * pCoefs->b[1];
    acc_b = acc_b + pStates[0].x2 * pCoefs->b[2];
    acc_b = acc_b * pCoefs->shifts_a_mul;
    pStates[0].x2 = pStates[0].x1;
    pStates[0].x1 = acc_a;
    pStates[0].NS = 0;
    *pOut = acc_b;
}

static void BiquadMonoFloatProcessing(const float_t *pIn, float_t *pOut, const BiquadFloatMonoCoefs_t *pCoefs, BiquadFloatMonoStates_t *pStates)
{
    if (pCoefs->active)
    {
        BiquadMonoFloatProcessingSimple(pIn, pOut, pCoefs, pStates);
    }
    else
    {
        *pOut = *pIn;
    }
}

static void BiquadStereoFloatProcessingSimple(const float_t *pIn, float_t *pOut, const BiquadFloatStereoCoefs_t *pCoefs, BiquadFloatStereoStates_t *pStates)
{
    float2_t acc;
    float2_t acc_a = {0, 0};
    float2_t acc_b = {0, 0};
    float2_t dIn;

    dIn.a = (float_t)pIn[0];
    dIn.b = (float_t)pIn[1];
    acc.a = pStates[0].NS.a;
    acc.b = pStates[0].NS.b;
    acc_a.a = dIn.a * pCoefs->a[0].a;
    acc_a.b = dIn.b * pCoefs->a[0].b;
    acc_a.a = acc_a.a + pStates[0].x1.a * pCoefs->a[1].a;
    acc_a.b = acc_a.b + pStates[0].x1.b * pCoefs->a[1].b;
    acc_a.a = acc_a.a + pStates[0].x2.a * pCoefs->a[2].a;
    acc_a.b = acc_a.b + pStates[0].x2.b * pCoefs->a[2].b;
    acc_a.a = acc_a.a * pCoefs->shifts_a_mul.a;
    acc_a.b = acc_a.b * pCoefs->shifts_a_mul.b;
    acc_b.a = acc_a.a * pCoefs->b[0].a;
    acc_b.b = acc_a.b * pCoefs->b[0].b;
    acc_b.a = acc_b.a + pStates[0].x1.a * pCoefs->b[1].a;
    acc_b.b = acc_b.b + pStates[0].x1.b * pCoefs->b[1].b;
    acc_b.a = acc_b.a + pStates[0].x2.a * pCoefs->b[2].a;
    acc_b.b = acc_b.b + pStates[0].x2.b * pCoefs->b[2].b;
    acc_b.a = acc_b.a * pCoefs->shifts_b_mul.a;
    acc_b.b = acc_b.b * pCoefs->shifts_b_mul.b;
    pStates[0].x2 = pStates[0].x1;
    pStates[0].x1 = acc_a;
    pStates[0].NS.a = 0;
    pStates[0].NS.b = 0;
    pOut[0] = acc_b.a;
    pOut[1] = acc_b.b;
}

static void BiquadStereoFloatProcessing(const float_t *pIn, float_t *pOut, const BiquadFloatStereoCoefs_t *pCoefs, BiquadFloatStereoStates_t *pStates)
{
    if (pCoefs->active)
    {
        BiquadStereoFloatProcessingSimple(pIn, pOut, pCoefs, pStates);
    }
    else
    {
        pOut[0] = pIn[0];
        pOut[1] = pIn[1];
    }
}

static intg2_t EQCoefsFloatNormalise(float *b, float *a)
{
    float max_a = 0;
    float max_b = 0;
    float a0 = a[0];
    intg_t shifts_a = 0;
    intg_t shifts_b = 0;

    for (intg_t i = 0; i < 3; i++)
    {
        b[i] /= a0;
        a[i] /= a0;
        if (fabsf(b[i]) > max_b)
            max_b = fabsf(b[i]);
        if (fabsf(a[i]) > max_a)
            max_a = fabsf(a[i]);
    }

    while (max_a > 1)
    {
        max_a /= 2;
        shifts_a++;
    }

    while (max_b > 1)
    {
        max_b /= 2;
        shifts_b++;
    }

    shifts_a += PEQ_COEFS_HEADROOM;
    shifts_b += PEQ_COEFS_HEADROOM;

    for (intg_t i = 0; i < 3; i++)
    {
        b[i] = b[i] * pow(2.0, -shifts_b);
        a[i] = a[i] * pow(2.0, -shifts_a);
    }

    return IntgVecMake(shifts_a, shifts_b);
}

static void SetBiquadCoefsFloatDefault(BiquadParam_t *pParams, BiquadFloatMonoCoefs_t *pCoefs, float sampleRate)
{
    pCoefs->active = 0;
    pCoefs->shifts_a = pCoefs->shifts_b = 0;
    pCoefs->shifts_a_mul = pCoefs->shifts_b_mul = 1.0f;
    pCoefs->b[0] = 1.f;
    pCoefs->b[1] = 0;
    pCoefs->b[2] = 0;
    pCoefs->a[0] = 1.f;
    pCoefs->a[1] = 0;
    pCoefs->a[2] = 0;
}

static void SetBiquadCoefsFloatSimple(BiquadParam_t *pParams, BiquadFloatMonoCoefs_t *pCoefs, float sampleRate)
{
    float Fc = pParams->Frequency;
    float Q = pParams->Q;
    float dbGain = pParams->Gain;
    float w = 2 * DOUBLE_PI * Fc / sampleRate;
    float A = pow(10.0, dbGain / 40);
    float alpha = sin(w) / (2 * Q);

    float a[3] = {1, 0, 0};
    float b[3] = {1, 0, 0};

    switch (pParams->Type)
    {
    case EQ_TYPE_LPF:
        // LPF:        H(s) = 1 / (s^2 + s/Q + 1)
        //             b0 =  (1 - cos(w0))/2
        //             b1 =   1 - cos(w0)
        //             b2 =  (1 - cos(w0))/2
        //             a0 =   1 + alpha
        //             a1 =  -2*cos(w0)
        //             a2 =   1 - alpha
        b[0] = (1 - cos(w)) / 2;
        b[1] = (1 - cos(w));
        b[2] = (1 - cos(w)) / 2;

        a[0] = 1 + alpha;
        a[1] = -2 * cos(w);
        a[2] = 1 - alpha;
        break;

    case EQ_TYPE_HPF:
        // HPF:        H(s) = s^2 / (s^2 + s/Q + 1)
        //             b0 =  (1 + cos(w0))/2
        //             b1 = -(1 + cos(w0))
        //             b2 =  (1 + cos(w0))/2
        //             a0 =   1 + alpha
        //             a1 =  -2*cos(w0)
        //             a2 =   1 - alpha
        b[0] = (1 + cos(w)) / 2;
        b[1] = -(1 + cos(w));
        b[2] = (1 + cos(w)) / 2;

        a[0] = 1 + alpha;
        a[1] = -2 * cos(w);
        a[2] = 1 - alpha;
        break;

    case EQ_TYPE_BP:
        // BPF:        H(s) = s / (s^2 + s/Q + 1)  (constant skirt gain, peak gain = Q)
        //             b0 =   sin(w0)/2  =   Q*alpha
        //             b1 =   0
        //             b2 =  -sin(w0)/2  =  -Q*alpha
        //             a0 =   1 + alpha
        //             a1 =  -2*cos(w0)
        //             a2 =   1 - alpha
        b[0] = sin(w) / 2;
        b[1] = 0;
        b[2] = -sin(w) / 2;

        a[0] = 1 + alpha;
        a[1] = -2 * cos(w);
        a[2] = 1 - alpha;
        break;

    case EQ_TYPE_BP0DB:
        // BPF:        H(s) = (s/Q) / (s^2 + s/Q + 1)      (constant 0 dB peak gain)
        //             b0 =   alpha
        //             b1 =   0
        //             b2 =  -alpha
        //             a0 =   1 + alpha
        //             a1 =  -2*cos(w0)
        //             a2 =   1 - alpha
        b[0] = alpha;
        b[1] = 0;
        b[2] = -alpha;

        a[0] = 1 + alpha;
        a[1] = -2 * cos(w);
        a[2] = 1 - alpha;
        break;

    case EQ_TYPE_NOTCH:
        // notch:      H(s) = (s^2 + 1) / (s^2 + s/Q + 1)
        //             b0 =   1
        //             b1 =  -2*cos(w0)
        //             b2 =   1
        //             a0 =   1 + alpha
        //             a1 =  -2*cos(w0)
        //             a2 =   1 - alpha
        b[0] = 1;
        b[1] = -2 * cos(w);
        b[2] = 1;

        a[0] = 1 + alpha;
        a[1] = -2 * cos(w);
        a[2] = 1 - alpha;
        break;

    case EQ_TYPE_PEAK:
        // peakingEQ:  H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
        //             b0 =   1 + alpha*A
        //             b1 =  -2*cos(w0)
        //             b2 =   1 - alpha*A
        //             a0 =   1 + alpha/A
        //             a1 =  -2*cos(w0)
        //             a2 =   1 - alpha/A
        b[0] = 1 + alpha * A;
        b[1] = -2 * cos(w);
        b[2] = 1 - alpha * A;

        a[0] = 1 + alpha / A;
        a[1] = -2 * cos(w);
        a[2] = 1 - alpha / A;
        break;

    case EQ_TYPE_LSHELF:
        // lowShelf: H(s) = A * (s^2 + (sqrt(A)/Q)*s + A)/(A*s^2 + (sqrt(A)/Q)*s + 1)
        //             b0 =    A*( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha )
        //             b1 =  2*A*( (A-1) - (A+1)*cos(w0)                   )
        //             b2 =    A*( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha )
        //             a0 =        (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha
        //             a1 =   -2*( (A-1) + (A+1)*cos(w0)                   )
        //             a2 =        (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha
        b[0] = A * ((A + 1) - (A - 1) * cos(w) + 2 * sqrt(A) * alpha);
        b[1] = 2 * A * ((A - 1) - (A + 1) * cos(w));
        b[2] = A * ((A + 1) - (A - 1) * cos(w) - 2 * sqrt(A) * alpha);

        a[0] = (A + 1) + (A - 1) * cos(w) + 2 * sqrt(A) * alpha;
        a[1] = -2 * ((A - 1) + (A + 1) * cos(w));
        a[2] = (A + 1) + (A - 1) * cos(w) - 2 * sqrt(A) * alpha;
        break;

    case EQ_TYPE_HSHELF:
        // highShelf: H(s) = A * (A*s^2 + (sqrt(A)/Q)*s + 1)/(s^2 + (sqrt(A)/Q)*s + A)
        //             b0 =    A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha )
        //             b1 = -2*A*( (A-1) + (A+1)*cos(w0)                   )
        //             b2 =    A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha )
        //             a0 =        (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha
        //             a1 =    2*( (A-1) - (A+1)*cos(w0)                   )
        //             a2 =        (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha
        b[0] = A * ((A + 1) + (A - 1) * cos(w) + 2 * sqrt(A) * alpha);
        b[1] = -2 * A * ((A - 1) + (A + 1) * cos(w));
        b[2] = A * ((A + 1) + (A - 1) * cos(w) - 2 * sqrt(A) * alpha);

        a[0] = (A + 1) - (A - 1) * cos(w) + 2 * sqrt(A) * alpha;
        a[1] = 2 * ((A - 1) - (A + 1) * cos(w));
        a[2] = (A + 1) - (A - 1) * cos(w) - 2 * sqrt(A) * alpha;
        break;

    case EQ_TYPE_APF:
        // APF:        H(s) = (s^2 - s/Q + 1) / (s^2 + s/Q + 1)

        //            b0 =   1 - alpha
        //            b1 =  -2*cos(w0)
        //            b2 =   1 + alpha
        //            a0 =   1 + alpha
        //            a1 =  -2*cos(w0)
        //            a2 =   1 - alpha
        b[0] = 1 - alpha;
        b[1] = -2 * cos(w);
        b[2] = 1 + alpha;

        a[0] = 1 + alpha;
        a[1] = -2 * cos(w);
        a[2] = 1 - alpha;
        break;
    }
    intg2_t shifts = EQCoefsFloatNormalise(b, a);
    if (pParams->Active)
    {
        pCoefs->shifts_a = shifts.a;
        pCoefs->shifts_b = shifts.b;
        pCoefs->shifts_a_mul = pow(2.0f, shifts.a); // Approch using multiplication insted shifting
        pCoefs->shifts_b_mul = pow(2.0f, shifts.b);
    }
    else
    {
        pCoefs->shifts_a = pCoefs->shifts_b = 0;
        pCoefs->shifts_a_mul = pCoefs->shifts_b_mul = 1.0f;
    }

    pCoefs->shifts_b = pCoefs->shifts_a;
    pCoefs->b[0] = b[0];
    pCoefs->b[1] = b[1];
    pCoefs->b[2] = b[2];
    pCoefs->a[1] = -a[1];
    pCoefs->a[2] = -a[2];
    pCoefs->a[0] = a[0];

    pCoefs->active = pParams->Active;    
}

static void SetBiquadCoefsFloat(BiquadParam_t *pParams, BiquadFloatMonoCoefs_t *pCoefs, float sampleRate)
{
    float Fc = pParams->Frequency;
    float Q = pParams->Q;
    float dbGain = pParams->Gain;
    float w = 2 * DOUBLE_PI * Fc / sampleRate;
    float A = pow(10.0, dbGain / 40);
    float alpha = sin(w) / (2 * Q);

    float a[3] = {1, 0, 0};
    float b[3] = {1, 0, 0};

    if (pParams->Active)
    {
        switch (pParams->Type)
        {
        case EQ_TYPE_LPF:
            // LPF:        H(s) = 1 / (s^2 + s/Q + 1)
            //             b0 =  (1 - cos(w0))/2
            //             b1 =   1 - cos(w0)
            //             b2 =  (1 - cos(w0))/2
            //             a0 =   1 + alpha
            //             a1 =  -2*cos(w0)
            //             a2 =   1 - alpha
            b[0] = (1 - cos(w)) / 2;
            b[1] = (1 - cos(w));
            b[2] = (1 - cos(w)) / 2;

            a[0] = 1 + alpha;
            a[1] = -2 * cos(w);
            a[2] = 1 - alpha;
            break;

        case EQ_TYPE_HPF:
            // HPF:        H(s) = s^2 / (s^2 + s/Q + 1)
            //             b0 =  (1 + cos(w0))/2
            //             b1 = -(1 + cos(w0))
            //             b2 =  (1 + cos(w0))/2
            //             a0 =   1 + alpha
            //             a1 =  -2*cos(w0)
            //             a2 =   1 - alpha
            b[0] = (1 + cos(w)) / 2;
            b[1] = -(1 + cos(w));
            b[2] = (1 + cos(w)) / 2;

            a[0] = 1 + alpha;
            a[1] = -2 * cos(w);
            a[2] = 1 - alpha;
            break;

        case EQ_TYPE_BP:
            // BPF:        H(s) = s / (s^2 + s/Q + 1)  (constant skirt gain, peak gain = Q)
            //             b0 =   sin(w0)/2  =   Q*alpha
            //             b1 =   0
            //             b2 =  -sin(w0)/2  =  -Q*alpha
            //             a0 =   1 + alpha
            //             a1 =  -2*cos(w0)
            //             a2 =   1 - alpha
            b[0] = sin(w) / 2;
            b[1] = 0;
            b[2] = -sin(w) / 2;

            a[0] = 1 + alpha;
            a[1] = -2 * cos(w);
            a[2] = 1 - alpha;
            break;

        case EQ_TYPE_BP0DB:
            // BPF:        H(s) = (s/Q) / (s^2 + s/Q + 1)      (constant 0 dB peak gain)
            //             b0 =   alpha
            //             b1 =   0
            //             b2 =  -alpha
            //             a0 =   1 + alpha
            //             a1 =  -2*cos(w0)
            //             a2 =   1 - alpha
            b[0] = alpha;
            b[1] = 0;
            b[2] = -alpha;

            a[0] = 1 + alpha;
            a[1] = -2 * cos(w);
            a[2] = 1 - alpha;
            break;

        case EQ_TYPE_NOTCH:
            // notch:      H(s) = (s^2 + 1) / (s^2 + s/Q + 1)
            //             b0 =   1
            //             b1 =  -2*cos(w0)
            //             b2 =   1
            //             a0 =   1 + alpha
            //             a1 =  -2*cos(w0)
            //             a2 =   1 - alpha
            b[0] = 1;
            b[1] = -2 * cos(w);
            b[2] = 1;

            a[0] = 1 + alpha;
            a[1] = -2 * cos(w);
            a[2] = 1 - alpha;
            break;

        case EQ_TYPE_PEAK:
            // peakingEQ:  H(s) = (s^2 + s*(A/Q) + 1) / (s^2 + s/(A*Q) + 1)
            //             b0 =   1 + alpha*A
            //             b1 =  -2*cos(w0)
            //             b2 =   1 - alpha*A
            //             a0 =   1 + alpha/A
            //             a1 =  -2*cos(w0)
            //             a2 =   1 - alpha/A
            b[0] = 1 + alpha * A;
            b[1] = -2 * cos(w);
            b[2] = 1 - alpha * A;

            a[0] = 1 + alpha / A;
            a[1] = -2 * cos(w);
            a[2] = 1 - alpha / A;
            break;

        case EQ_TYPE_LSHELF:
            // lowShelf: H(s) = A * (s^2 + (sqrt(A)/Q)*s + A)/(A*s^2 + (sqrt(A)/Q)*s + 1)
            //             b0 =    A*( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha )
            //             b1 =  2*A*( (A-1) - (A+1)*cos(w0)                   )
            //             b2 =    A*( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha )
            //             a0 =        (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha
            //             a1 =   -2*( (A-1) + (A+1)*cos(w0)                   )
            //             a2 =        (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha
            b[0] = A * ((A + 1) - (A - 1) * cos(w) + 2 * sqrt(A) * alpha);
            b[1] = 2 * A * ((A - 1) - (A + 1) * cos(w));
            b[2] = A * ((A + 1) - (A - 1) * cos(w) - 2 * sqrt(A) * alpha);

            a[0] = (A + 1) + (A - 1) * cos(w) + 2 * sqrt(A) * alpha;
            a[1] = -2 * ((A - 1) + (A + 1) * cos(w));
            a[2] = (A + 1) + (A - 1) * cos(w) - 2 * sqrt(A) * alpha;
            break;

        case EQ_TYPE_HSHELF:
            // highShelf: H(s) = A * (A*s^2 + (sqrt(A)/Q)*s + 1)/(s^2 + (sqrt(A)/Q)*s + A)
            //             b0 =    A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha )
            //             b1 = -2*A*( (A-1) + (A+1)*cos(w0)                   )
            //             b2 =    A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha )
            //             a0 =        (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha
            //             a1 =    2*( (A-1) - (A+1)*cos(w0)                   )
            //             a2 =        (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha
            b[0] = A * ((A + 1) + (A - 1) * cos(w) + 2 * sqrt(A) * alpha);
            b[1] = -2 * A * ((A - 1) + (A + 1) * cos(w));
            b[2] = A * ((A + 1) + (A - 1) * cos(w) - 2 * sqrt(A) * alpha);

            a[0] = (A + 1) - (A - 1) * cos(w) + 2 * sqrt(A) * alpha;
            a[1] = 2 * ((A - 1) - (A + 1) * cos(w));
            a[2] = (A + 1) - (A - 1) * cos(w) - 2 * sqrt(A) * alpha;
            break;

        case EQ_TYPE_APF:
            // APF:        H(s) = (s^2 - s/Q + 1) / (s^2 + s/Q + 1)

            //            b0 =   1 - alpha
            //            b1 =  -2*cos(w0)
            //            b2 =   1 + alpha
            //            a0 =   1 + alpha
            //            a1 =  -2*cos(w0)
            //            a2 =   1 - alpha
            b[0] = 1 - alpha;
            b[1] = -2 * cos(w);
            b[2] = 1 + alpha;

            a[0] = 1 + alpha;
            a[1] = -2 * cos(w);
            a[2] = 1 - alpha;
            break;
        }
    }
    else
    {
        b[0] = 1.0f;
        b[1] = 0;
        b[2] = 0;

        a[0] = 1.0f;
        a[1] = 0;
        a[2] = 0;
    }

    intg2_t shifts = EQCoefsFloatNormalise(b, a);
    if (pParams->Active)
    {
        pCoefs->shifts_a = shifts.a;
        pCoefs->shifts_b = shifts.b;
        pCoefs->shifts_a_mul = pow(2.0f, shifts.a); // Approch using multiplication insted shifting
        pCoefs->shifts_b_mul = pow(2.0f, shifts.b);
    }
    else
    {
        pCoefs->shifts_a = pCoefs->shifts_b = 0;
        pCoefs->shifts_a_mul = pCoefs->shifts_b_mul = 1.0f;
    }

    pCoefs->shifts_b = pCoefs->shifts_a;
    pCoefs->b[0] = b[0];
    pCoefs->b[1] = b[1];
    pCoefs->b[2] = b[2];
    pCoefs->a[1] = -a[1];
    pCoefs->a[2] = -a[2];
    pCoefs->a[0] = a[0];

    pCoefs->active = pParams->Active;
}

#endif // __BASIC_EQ_MATH_H__
