#ifndef COMPRESSOR_THREAD_H
#define COMPRESSOR_THREAD_H
#include "structures/fifo_thread_safe.h"

typedef struct {
    fifo_t *fifo_to_read;
    fifo_t *fifo_to_write;
} compressor_thread_args_t;

compressor_thread_args_t* compressor_thread_args_create(fifo_t *fifo_to_read, fifo_t* fifo_to_write);

void* run_compressor_thread(void *compressor_thread_args);

void compressor_thread_args_destroy(compressor_thread_args_t *compressor_thread_args);


#endif