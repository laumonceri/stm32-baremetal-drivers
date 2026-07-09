#include "ring_buffer.h"

void RingBuffer_Init(RingBuffer *rb) {
  rb->head = 0;
  rb->tail = 0;
}

int RingBuffer_IsEmpty(const RingBuffer *rb) { return rb->head == rb->tail; }

int RingBuffer_IsFull(const RingBuffer *rb) {
  return ((rb->head + 1) % RING_BUFFER_SIZE) == rb->tail;
}

// Move head to the next slot
void RingBuffer_Push(RingBuffer *rb, uint8_t data) {
  if (!RingBuffer_IsFull(rb)) {
    rb->buffer[rb->head] = data;
    // Update head index with wrap-around
    rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
  }
}

// Move tail to the next slot
int RingBuffer_Pop(RingBuffer *rb, uint8_t *data) {
  if (RingBuffer_IsEmpty(rb)) {
    return 0;
  }

  *data = rb->buffer[rb->tail];
  // Update tail index with wrap-around
  rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
  return 1;
}
