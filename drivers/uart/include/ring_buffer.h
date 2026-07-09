#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

#define RING_BUFFER_SIZE 128

typedef struct {
    volatile uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint16_t head; // write index
    volatile uint16_t tail; // read index
} RingBuffer;

/**
 * @brief Reset a buffer to empty.
 * @param rb Buffer to reset.
 */
void RingBuffer_Init(RingBuffer *rb);

/**
 * @brief Check whether the buffer holds no data.
 * @param rb Buffer to check.
 * @return Non-zero if empty.
 */
int RingBuffer_IsEmpty(const RingBuffer *rb);

/**
 * @brief Check whether the buffer has no room for another byte.
 * @param rb Buffer to check.
 * @return Non-zero if full.
 */
int RingBuffer_IsFull(const RingBuffer *rb);

/**
 * @brief Push one byte. No-op if the buffer is full (data is dropped).
 * @param rb   Buffer to push into.
 * @param data Byte to push.
 */
void RingBuffer_Push(RingBuffer *rb, uint8_t data);

/**
 * @brief Pop one byte.
 * @param rb   Buffer to pop from.
 * @param data Out param, written only on success.
 * @return 1 if a byte was popped, 0 if the buffer was empty.
 */
int RingBuffer_Pop(RingBuffer *rb, uint8_t *data);

#endif
