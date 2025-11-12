#ifndef FIFO_TO_STDOUT_THREAD_H
#define FIFO_TO_STDOUT_THREAD_H
#include "structures/fifo_thread_safe.h"

typedef struct {
    fifo_t *fifo_to_read;
} fifo_to_stdout_thread_args_t;

fifo_to_stdout_thread_args_t* fifo_to_stdout_thread_args_create(fifo_t* fifo_to_read);

void* run_fifo_to_stdout_thread(void *fifo_to_stdout_thread_args);

void fifo_to_stdout_thread_args_destroy(fifo_to_stdout_thread_args_t *fifo_to_stdout_thread_args);

#endif