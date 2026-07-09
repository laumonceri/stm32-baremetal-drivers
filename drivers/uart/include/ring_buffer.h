#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

#define RING_BUFFER_SIZE 128

typedef struct {
    volatile uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint16_t head; // write index
    volatile uint16_t tail; // read index
} RingBuffer;

void RingBuffer_Init(RingBuffer *rb);
int RingBuffer_IsEmpty(const RingBuffer *rb);
int RingBuffer_IsFull(const RingBuffer *rb);
void RingBuffer_Push(RingBuffer *rb, uint8_t data);
int RingBuffer_Pop(RingBuffer *rb, uint8_t *data);

#endif
