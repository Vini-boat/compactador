#ifndef COMPRESSOR_THREAD_H
#define COMPRESSOR_THREAD_H
#include "structures/fifo_thread_safe.h"
#include "structures/stats_shm.h"
typedef struct {
    stats_shm_t *stats;
    fifo_t *fifo_to_read;
    fifo_t *fifo_to_write;
} compressor_thread_args_t;

compressor_thread_args_t* compressor_thread_args_create(stats_shm_t *stats,fifo_t *fifo_to_read, fifo_t* fifo_to_write);

void* run_compressor_thread(void *compressor_thread_args);

void compressor_thread_args_destroy(compressor_thread_args_t *compressor_thread_args);


#endif