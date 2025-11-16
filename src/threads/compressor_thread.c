#ifndef COMPRESSOR_THREAD_C
#define COMPRESSOR_THREAD_C
#include "threads/compressor_thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures/fifo_thread_safe.h"
#include "structures/dictionary.h"


compressor_thread_args_t* compressor_thread_args_create(fifo_t *fifo_to_read, fifo_t* fifo_to_write){
    compressor_thread_args_t *args = (compressor_thread_args_t*) malloc(sizeof(compressor_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em compressor_thread_args_create");
        return NULL;
    }

    args->fifo_to_read = fifo_to_read;
    args->fifo_to_write = fifo_to_write;

    return args;
}

void* run_compressor_thread(void *compressor_thread_args){
    compressor_thread_args_t *args = (compressor_thread_args_t *) compressor_thread_args;
    
    dict_t *dict = dict_create();
    dict_add_default_values(dict);

    char token_buffer[FIFO_MAX_WORD_LENGTH];
    char code_string[2] = "#"; 
    
    while(1){
        fifo_pop(args->fifo_to_read, token_buffer);
        if(strcmp(token_buffer,FIFO_FINAL_TOKEN) == 0) break;

        if(dict_get_code(dict,token_buffer, &code_string[0]) != -1){
            fifo_push(args->fifo_to_write,code_string);
        } else {
            fifo_push(args->fifo_to_write,token_buffer);
        }

    }
    fifo_push(args->fifo_to_write, FIFO_FINAL_TOKEN);

    dict_destroy(dict);
    return NULL;
}

void compressor_thread_args_destroy(compressor_thread_args_t *args){
    free(args);
}


#endif