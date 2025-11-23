#ifndef DECOMPRESSOR_THREAD_H
#define DECOMPRESSOR_THREAD_H
#include "structures/fifo_thread_safe.h"
#include "structures/stats_shm.h"

typedef struct {
    stats_shm_t *stats;
    fifo_t *fifo_to_read;
    fifo_t *fifo_to_write;
} decompressor_thread_args_t;

decompressor_thread_args_t* decompressor_thread_args_create(stats_shm_t *stats, fifo_t *fifo_to_read, fifo_t* fifo_to_write);

void* run_decompressor_thread(void *decompressor_thread_args);

void decompressor_thread_args_destroy(decompressor_thread_args_t *decompressor_thread_args);

#endif