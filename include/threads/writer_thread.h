#ifndef WRITER_THREAD_H
#define WRITER_THREAD_H
#include "structures/fifo_thread_safe.h"

typedef struct {
    char *filename;
    fifo_t *fifo_to_read;
} writer_thread_args_t;

writer_thread_args_t* writer_thread_args_create(const char* filename, fifo_t* fifo_to_read);

void* run_writer_thread(void *writer_thread_args);

void writer_thread_args_destroy(writer_thread_args_t *writer_thread_args);

#endif