#ifndef READER_THREAD_H
#define READER_THREAD_H
#include "structures/fifo_thread_safe.h"

typedef struct {
    char *filename;
    fifo_t *fifo_to_write;
} reader_thread_args_t;

reader_thread_args_t* reader_thread_args_create(const char* filename, fifo_t* fifo_to_write);

void* run_reader_thread(void *reader_thread_args);

void reader_thread_args_destroy(reader_thread_args_t *reader_thread_args);

#endif