#ifndef COMPRESSOR_THREAD_C
#define COMPRESSOR_THREAD_C
#include "threads/compressor_thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures/fifo_thread_safe.h"
#include "structures/dictionary.h"
#include "structures/stats_shm.h"

compressor_thread_args_t* compressor_thread_args_create(stats_shm_t *stats,fifo_t *fifo_to_read, fifo_t* fifo_to_write){
    compressor_thread_args_t *args = (compressor_thread_args_t*) malloc(sizeof(compressor_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em compressor_thread_args_create");
        return NULL;
    }

    args->stats = stats;
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
        int read_tokens = 0;
        int compressed_tokens = 0;
        int tokens_not_in_dict = 0;

        fifo_pop(args->fifo_to_read, token_buffer);
        if(strcmp(token_buffer,FIFO_FINAL_TOKEN) == 0) break;
        read_tokens +=1;

        if(dict_get_code(dict,token_buffer, &code_string[0]) != -1){
            fifo_push(args->fifo_to_write,code_string);
            compressed_tokens += 1;
        } else {
            fifo_push(args->fifo_to_write,token_buffer);
            tokens_not_in_dict += 1;
        }
        stats_shm_comp_update(args->stats,read_tokens,compressed_tokens,tokens_not_in_dict);
    }
    fifo_push(args->fifo_to_write, FIFO_FINAL_TOKEN);

    dict_destroy(dict);
    return NULL;
}

void compressor_thread_args_destroy(compressor_thread_args_t *args){
    free(args);
}


#endif