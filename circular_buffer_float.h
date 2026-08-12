#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include "basic_types.h"

#define CircularBufferElement_t float_t

typedef struct
{
    int start;
    int end;
	int size;
    CircularBufferElement_t* elements;
} CircularBuffer_t;

#define DECLARE_CIRCULAR_BUFFER(inst_name, N) \
    CircularBufferElement_t inst_name##_array[(N)]; \
    CircularBuffer_t inst_name

#define INIT_CIRCULAR_BUFFER(inst_name) \
    inst_name.start = 0; \
    inst_name.end = 0; \
    inst_name.size = sizeof(inst_name##_array) / sizeof(CircularBufferElement_t); \
    inst_name.elements = inst_name##_array; \
    for (int i = 0; i < inst_name.size; i++) { \
        inst_name.elements[i] = 0; \
    }    

static void CircularBufferSetSize(CircularBuffer_t* pBuffer, intg_t num) {
    pBuffer->size = num;
}

static void CircularBufferSetTail(CircularBuffer_t* pBuffer, intg_t pos) {
    pBuffer->end = pos;
}

static void CircularBufferSetFull(CircularBuffer_t* pBuffer) {
    pBuffer->end = pBuffer->size - 1;
}

static void CircularBufferInit(CircularBuffer_t* pBuffer) {
    pBuffer->start = 0;
    pBuffer->end = 0;
    for (int i = 0; i < pBuffer->size; i++) {
        pBuffer->elements[i] = 0;
    }
}

static void CircularBufferInitFull(CircularBuffer_t* pBuffer) {
    pBuffer->start = 0;
    pBuffer->end = pBuffer->size - 1;
    for (int i = 0; i < pBuffer->size; i++) {
        pBuffer->elements[i] = 0;
    }
}

static void CircularBufferFill(CircularBuffer_t* pBuffer, CircularBufferElement_t value) {
    pBuffer->start = 0;
    pBuffer->end = 0;
    for (int i = 0; i < pBuffer->size; i++) {
        pBuffer->elements[i] = value;
    }
}

static void CircularBufferPush(CircularBuffer_t* pBuffer, CircularBufferElement_t* value) {
    pBuffer->elements[pBuffer->end] = *value;
    pBuffer->end++;
    if (pBuffer->end >= pBuffer->size) {
        pBuffer->end = 0;
    }
    if (pBuffer->end == pBuffer->start) {
        pBuffer->start++;
        if (pBuffer->start >= pBuffer->size) {
            pBuffer->start = 0;
        }
    }
}

static CircularBufferElement_t CircularBufferPop(CircularBuffer_t* pBuffer){
    CircularBufferElement_t result = 0;
    if (pBuffer->start == pBuffer->end) {
        return result;
    }
    result = pBuffer->elements[pBuffer->start];
    pBuffer->start++;
    if (pBuffer->start >= pBuffer->size) {
        pBuffer->start = 0;
    }
    return result;
}

static CircularBufferElement_t CircularBufferGet(const CircularBuffer_t* pBuffer, int index) {
    int idx = pBuffer->start + index;
    if (idx >= pBuffer->size) {
        idx -= pBuffer->size;
    }
    return pBuffer->elements[idx];
}

static CircularBufferElement_t CircularBufferGetFromTail(const CircularBuffer_t* pBuffer, int index) {
    int idx = pBuffer->end - 1 - index;
    if (idx < 0) {
        idx += pBuffer->size;
    }
    return pBuffer->elements[idx];
}


static void CircularBufferSet(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int index) {
    int idx = pBuffer->start + index;
    if (idx >= pBuffer->size) {
        idx -= pBuffer->size;
    }
    pBuffer->elements[idx] = *value;
}

static void CircularBufferPushChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size){
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &value[i]);
    }
}

static void CircularBufferPushChunkInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &value[2 * i]);
    }
}

static void CircularBufferPushChunkDualMonoToInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* valueL, CircularBufferElement_t* valueR, int size) {
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &valueL[i]);
        CircularBufferPush(pBuffer, &valueR[i]);
    }
}

static void CircularBufferGetChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size){
    for (int i = 0; i < size; i++) {
        value[i] = CircularBufferGet(pBuffer, i);
    }
}

static CircularBufferElement_t CircularBufferGetChunkSum(CircularBuffer_t* pBuffer, int size) {
    CircularBufferElement_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += CircularBufferGet(pBuffer, i);
    }
	return sum;
}

static CircularBufferElement_t CircularBufferGetAbsSumInterleaved(CircularBuffer_t* pBuffer, int size, int offset) {
    CircularBufferElement_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += fabsf(CircularBufferGet(pBuffer, 2 * i + offset));
    }
    return sum;
}

static CircularBufferElement_t CircularBufferGetAbsSumInterleavedFromTail(CircularBuffer_t* pBuffer, int size, int offset) {
    CircularBufferElement_t sum = 0;
    for (int i = 0; i < size; i++) {
        sum += fabsf(CircularBufferGetFromTail(pBuffer, 2 * i + offset));
    }
    return sum;
}

static CircularBufferElement_t CircularBufferGetPeakInRangeInterleavedFromTail(CircularBuffer_t* pBuffer, int size, int offset) {
    CircularBufferElement_t maxVal = 0;
    CircularBufferElement_t val;
    for (int i = 0; i < size; i++) {
        val = fabsf(CircularBufferGetFromTail(pBuffer, 2 * i + offset));
        if (maxVal < val){
            maxVal = val;
        }
    }
    return maxVal;
}

static CircularBufferElement_t CircularBufferGetPeakInRangeFromTail(CircularBuffer_t* pBuffer, int size) {
    CircularBufferElement_t maxVal = 0;
    CircularBufferElement_t val;
    for (int i = 0; i < size; i++) {
        val = fabsf(CircularBufferGetFromTail(pBuffer, i));
        if (maxVal < val) {
            maxVal = val;
        }
    }
    return maxVal;
}

static CircularBufferElement_t CircularBufferGetPeakInRange(CircularBuffer_t* pBuffer, int size) {
    CircularBufferElement_t maxVal = 0;
    CircularBufferElement_t val;
    for (int i = 0; i < size; i++) {
        val = fabsf(CircularBufferGet(pBuffer, i));
        if (maxVal < val) {
            maxVal = val;
        }
    }
    return maxVal;
}

static CircularBufferElement_t CircularBufferGetPeakInRangeInterleaved(CircularBuffer_t* pBuffer, int size, int offset) {
    CircularBufferElement_t maxVal = 0;
    CircularBufferElement_t val;
    for (int i = 0; i < size; i += 2) {
        val = fabsf(CircularBufferGet(pBuffer, i + offset));
        if (maxVal < val) {
            maxVal = val;
        }
    }
    return maxVal;
}

static void CircularBufferPopChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        value[i] = CircularBufferPop(pBuffer);
    }
}

static void CircularBufferPopChunkScaled(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, float scale, int size) {
    for (int i = 0; i < size; i++) {
        value[i] = CircularBufferPop(pBuffer) * scale;
    }
}

static void CircularBufferPopChunkScaledInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, float scale, int size) {
    for (int i = 0; i < size; i++) {
        value[2 * i] = CircularBufferPop(pBuffer) * scale;
        value[2 * i + 1] = 0;
    }
}

static void CircularBufferPopChunkScaledInterleavedMix(CircularBuffer_t* pBuffer, CircularBufferElement_t* in, CircularBufferElement_t* out, float scale1, float scale2, int size) {
    for (int i = 0; i < size; i++) {
        out[2 * i] = CircularBufferPop(pBuffer) * scale1 + in[2 * i] * scale2;
    }
}

static void CircularBufferPushZeros(CircularBuffer_t* pBuffer, int size) {
	CircularBufferElement_t zero = 0;
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &zero);
    }
}

static void CircularBufferGetChunkInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        value[2 * i] = CircularBufferGet(pBuffer, i);
        value[2 * i + 1] = 0;
    }
}

static void CircularBufferGetChunkInterleavedFromTail(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
		int ptr = size - 1 - i;
        value[2 * i] = CircularBufferGetFromTail(pBuffer, ptr);
        value[2 * i + 1] = 0;
    }
}

static void CircularBufferGetChunkInterleavedFromTailWithOffset(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size, int offset) {
    for (int i = 0; i < size; i++) {
        int ptr = offset + size - 1 - i;
        value[2 * i] = CircularBufferGetFromTail(pBuffer, ptr);
        value[2 * i + 1] = 0;
    }
}


static void CircularBufferPopChunkInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        value[2 * i] = CircularBufferPop(pBuffer);
    }
}

static void CircularBufferPushChunkDeinterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < (size >> 1); i++) {
        CircularBufferPush(pBuffer, &value[2 * i]);
    }
}

static int CircularBufferGetSize(const CircularBuffer_t* pBuffer) {
    int size = pBuffer->end - pBuffer->start;
    if (size < 0) {
        size += pBuffer->size;
    }
    return size;
}

static CircularBufferElement_t CircularBufferPeek(CircularBuffer_t* pBuffer) {
    CircularBufferElement_t result = 0;

    if (pBuffer->start == pBuffer->end) {
        return result;
    }

    result = pBuffer->elements[pBuffer->start];

    pBuffer->start++;
    if (pBuffer->start >= pBuffer->size) {
        pBuffer->start = 0;
    }

    return result;
}

static void CircularBufferAddChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int pos, int size) {
    CircularBufferElement_t tmp;
    for (int i = 0; i < size; i++) {
        tmp = CircularBufferGet(pBuffer, pos + i);
        tmp = tmp + value[i];
        CircularBufferSet(pBuffer, &tmp, pos + i);
    }
}

#endif // __CIRCULAR_BUFFER_H__
