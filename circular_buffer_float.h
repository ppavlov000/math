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
    inst_name.elements = inst_name##_array

INLINE void CircularBufferSetSize(CircularBuffer_t* pBuffer, intg_t num) {
    pBuffer->size = num;
}

INLINE void CircularBufferInit(CircularBuffer_t* pBuffer) {
    pBuffer->start = 0;
    pBuffer->end = 0;
    for (int i = 0; i < pBuffer->size; i++) {
        pBuffer->elements[i] = 0;
    }
}

INLINE void CircularBufferInitFull(CircularBuffer_t* pBuffer) {
    pBuffer->start = 0;
    pBuffer->end = pBuffer->size - 1;
    for (int i = 0; i < pBuffer->size; i++) {
        pBuffer->elements[i] = 0;
    }
}

INLINE void CircularBufferPush(CircularBuffer_t* pBuffer, CircularBufferElement_t* value) {
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

INLINE CircularBufferElement_t CircularBufferPop(CircularBuffer_t* pBuffer){
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

INLINE CircularBufferElement_t CircularBufferGet(const CircularBuffer_t* pBuffer, int index) {
    int idx = pBuffer->start + index;
    if (idx >= pBuffer->size) {
        idx -= pBuffer->size;
    }
    return pBuffer->elements[idx];
}

INLINE CircularBufferElement_t CircularBufferGetFromTail(const CircularBuffer_t* pBuffer, int index) {
    int idx = pBuffer->end - 1 - index;
    if (idx < 0) {
        idx += pBuffer->size;
    }
    return pBuffer->elements[idx];
}


INLINE void CircularBufferSet(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int index) {
    int idx = pBuffer->start + index;
    if (idx >= pBuffer->size) {
        idx -= pBuffer->size;
    }
    pBuffer->elements[idx] = *value;
}

INLINE void CircularBufferPushChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size){
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &value[i]);
    }
}

INLINE void CircularBufferPushChunkInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &value[2 * i]);
    }
}

INLINE void CircularBufferPushChunkDualMonoToInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* valueL, CircularBufferElement_t* valueR, int size) {
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &valueL[i]);
        CircularBufferPush(pBuffer, &valueR[i]);
    }
}

INLINE void CircularBufferGetChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size){
    for (int i = 0; i < size; i++) {
        value[i] = CircularBufferGet(pBuffer, i);
    }
}

INLINE void CircularBufferPopChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        value[i] = CircularBufferPop(pBuffer);
    }
}

INLINE void CircularBufferPopChunkScaled(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, float scale, int size) {
    for (int i = 0; i < size; i++) {
        value[i] = CircularBufferPop(pBuffer) * scale;
    }
}

INLINE void CircularBufferPopChunkScaledInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, float scale, int size) {
    for (int i = 0; i < size; i++) {
        value[2 * i] = CircularBufferPop(pBuffer) * scale;
    }
}

INLINE void CircularBufferPopChunkScaledInterleavedMix(CircularBuffer_t* pBuffer, CircularBufferElement_t* in, CircularBufferElement_t* out, float scale1, float scale2, int size) {
    for (int i = 0; i < size; i++) {
        out[2 * i] = CircularBufferPop(pBuffer) * scale1 + in[2 * i] * scale2;
    }
}

//INLINE void CircularBufferPopChunkScaledInterleavedToInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, float scale, int size) {
//    for (int i = 0; i < size; i++) {
//        value[2 * i] = CircularBufferPop(pBuffer) * scale;
//    }
//}


INLINE void CircularBufferPushZeros(CircularBuffer_t* pBuffer, int size) {
	CircularBufferElement_t zero = 0;
    for (int i = 0; i < size; i++) {
        CircularBufferPush(pBuffer, &zero);
    }
}

INLINE void CircularBufferGetChunkInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        value[2 * i] = CircularBufferGet(pBuffer, i);
        value[2 * i + 1] = 0;
    }
}

INLINE void CircularBufferGetChunkInterleavedFromTail(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
		int ptr = size - 1 - i;
        value[2 * i] = CircularBufferGetFromTail(pBuffer, ptr);
        value[2 * i + 1] = 0;
    }
}

INLINE void CircularBufferGetChunkInterleavedFromTailWithOffset(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size, int offset) {
    for (int i = 0; i < size; i++) {
        int ptr = offset + size - 1 - i;
        value[2 * i] = CircularBufferGetFromTail(pBuffer, ptr);
        value[2 * i + 1] = 0;
    }
}


INLINE void CircularBufferPopChunkInterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < size; i++) {
        value[2 * i] = CircularBufferPop(pBuffer);
        value[2 * i + 1] = 0;
    }
}

INLINE void CircularBufferPushChunkDeinterleaved(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int size) {
    for (int i = 0; i < (size >> 1); i++) {
        CircularBufferPush(pBuffer, &value[2 * i]);
    }
}

INLINE int CircularBufferGetSize(const CircularBuffer_t* pBuffer) {
    int size = pBuffer->end - pBuffer->start;
    if (size < 0) {
        size += pBuffer->size;
    }
    return size;
}

INLINE CircularBufferElement_t CircularBufferPeek(CircularBuffer_t* pBuffer) {
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

INLINE void CircularBufferAddChunk(CircularBuffer_t* pBuffer, CircularBufferElement_t* value, int pos, int size) {
    CircularBufferElement_t tmp;
    for (int i = 0; i < size; i++) {
        tmp = CircularBufferGet(pBuffer, pos + i);
        tmp = tmp + value[i];
        CircularBufferSet(pBuffer, &tmp, pos + i);
    }
}

#endif // __CIRCULAR_BUFFER_H__
