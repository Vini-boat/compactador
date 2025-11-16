#ifndef TOKEN_READER_THREAD_H
#define TOKEN_READER_THREAD_H
#include "structures/fifo_thread_safe.h"

typedef struct {
    char *filename;
    fifo_t *fifo_to_write;
} token_reader_thread_args_t;

token_reader_thread_args_t* token_reader_thread_args_create(const char* filename, fifo_t* fifo_to_write);

void* run_token_reader_thread(void *token_reader_thread_args);

void token_reader_thread_args_destroy(token_reader_thread_args_t *token_reader_thread_args);

#endif