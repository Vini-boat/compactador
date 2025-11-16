#ifndef CODE_READER_THREAD_C
#define CODE_READER_THREAD_C
#include "threads/code_reader_thread.h"
#include "structures/fifo_thread_safe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

code_reader_thread_args_t* code_reader_thread_args_create(const char* filename, fifo_t* fifo_to_write){
    code_reader_thread_args_t *args = (code_reader_thread_args_t*) malloc(sizeof(code_reader_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em code_reader_thread_args_create");
        return NULL;
    }

    args->filename = strdup(filename); 
    if (args->filename == NULL) {
        perror("[ERROR] strdup em code_reader_thread_args_create");
        free(args);
        return NULL;
    }

    args->fifo_to_write = fifo_to_write;

    return args;
}

void* run_code_reader_thread(void *code_reader_thread_args){
    code_reader_thread_args_t *args = (code_reader_thread_args_t*) code_reader_thread_args;

    FILE *file = fopen(args->filename, "r");

    int c;
    char code[2] = "#";

    do {
        c = fgetc(file);
        if (c == EOF || c == 0x0) continue;

        code[0] = (char) c;
        fifo_push(args->fifo_to_write,code);

    } while (c != EOF);

    fclose(file);
    fifo_push(args->fifo_to_write, FIFO_FINAL_TOKEN);

    return NULL;
}

void code_reader_thread_args_destroy(code_reader_thread_args_t *args){
    free(args->filename);
    free(args);
}

#endif