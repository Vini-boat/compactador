#ifndef READER_THREAD_C
#define READER_THREAD_C
#include "threads/reader_thread.h"
#include "structures/fifo_thread_safe.h"

#include <stdio.h>
#include <stdlib.h>

reader_thread_args_t* reader_thread_args_create(const char* filename, fifo_t* fifo_to_write){
    reader_thread_args_t *args = (reader_thread_args_t*) malloc(sizeof(reader_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em reader_thread_args_create");
        return NULL;
    }

    args->filename = strdup(filename); 
    if (args->filename == NULL) {
        perror("[ERROR] strdup em reader_thread_args_create");
        free(args);
        return NULL;
    }


    args->fifo_to_write = fifo_to_write;

    return args;
}

void* run_reader_thread(void *reader_thread_args){
    reader_thread_args_t *args = (reader_thread_args_t*) reader_thread_args;

    FILE *file = fopen(args->filename, "r");

    char token[FIFO_MAX_WORD_LENGTH];

    // TODO: ajustar o algoritmo de tokenização
    while(fscanf(file, "%s", token) == 1){
        fifo_push(args->fifo_to_write, token);
    }

    fclose(file);
    fifo_push(args->fifo_to_write, FIFO_FINAL_TOKEN);

    return NULL;
}

void reader_thread_args_destroy(reader_thread_args_t *args){
    free(args->filename);
    free(args);
}

#endif