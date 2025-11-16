#ifndef CODE_READER_THREAD_H
#define CODE_READER_THREAD_H
#include "structures/fifo_thread_safe.h"

typedef struct {
    char *filename;
    fifo_t *fifo_to_write;
} code_reader_thread_args_t;

code_reader_thread_args_t* code_reader_thread_args_create(const char* filename, fifo_t* fifo_to_write);

void* run_code_reader_thread(void *code_reader_thread_args);

void code_reader_thread_args_destroy(code_reader_thread_args_t *code_reader_thread_args);

#endif