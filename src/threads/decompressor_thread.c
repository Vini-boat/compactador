#ifndef DECOMPRESSOR_THREAD_C
#define DECOMPRESSOR_THREAD_C
#include "threads/decompressor_thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures/fifo_thread_safe.h"
#include "structures/dictionary.h"
#include "structures/stats_shm.h"

decompressor_thread_args_t* decompressor_thread_args_create(stats_shm_t *stats,fifo_t *fifo_to_read, fifo_t* fifo_to_write){
    decompressor_thread_args_t *args = (decompressor_thread_args_t*) malloc(sizeof(decompressor_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em decompressor_thread_args_create");
        return NULL;
    }

    args->stats = stats;
    args->fifo_to_read = fifo_to_read;
    args->fifo_to_write = fifo_to_write;

    return args;
}

void* run_decompressor_thread(void *decompressor_thread_args){
    decompressor_thread_args_t *args = (decompressor_thread_args_t *) decompressor_thread_args;
    
    dict_t *dict = dict_create();
    dict_add_default_values(dict);

    char token_buffer[FIFO_MAX_WORD_LENGTH];
    char code_buffer[FIFO_MAX_WORD_LENGTH]; 
    
    while(1){
        int read_bytes = 0;
        int decompressed_bytes = 0;
        int bytes_not_in_dict = 0;


        fifo_pop(args->fifo_to_read, code_buffer);
        if(strcmp(code_buffer,FIFO_FINAL_TOKEN) == 0) break;
        read_bytes +=1;
        if(dict_get_token(dict,code_buffer[0], token_buffer) != -1){
            fifo_push(args->fifo_to_write,token_buffer);
            decompressed_bytes +=1;
        } else {
            fifo_push(args->fifo_to_write,code_buffer);
            bytes_not_in_dict +=1;
        }

        stats_shm_decomp_update(args->stats,read_bytes,decompressed_bytes,bytes_not_in_dict);

    }
    fifo_push(args->fifo_to_write, FIFO_FINAL_TOKEN);

    dict_destroy(dict);
    return NULL;
}

void decompressor_thread_args_destroy(decompressor_thread_args_t *args){
    free(args);
}

#endif