#ifndef FIFO_TO_STDOUT_THREAD_C
#define FIFO_TO_STDOUT_THREAD_C
#include "threads/fifo_to_stdout_thread.h"
#include "structures/fifo_thread_safe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

fifo_to_stdout_thread_args_t* fifo_to_stdout_thread_args_create(fifo_t* fifo_to_read){
    fifo_to_stdout_thread_args_t *args = (fifo_to_stdout_thread_args_t*) malloc(sizeof(fifo_to_stdout_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em reader_thread_args_create");
        return NULL;
    }

    args->fifo_to_read = fifo_to_read;

    return args;
}

void* run_fifo_to_stdout_thread(void *fifo_to_stdout_thread_args){
    fifo_to_stdout_thread_args_t *args = (fifo_to_stdout_thread_args_t*) fifo_to_stdout_thread_args;
    
    char buffer[FIFO_MAX_WORD_LENGTH];
    int i = 0;
    
    while(1){
        
        fifo_pop(args->fifo_to_read,buffer);
        
        if(strcmp(buffer, FIFO_FINAL_TOKEN) == 0) break;
        
        printf("token %d: %s\n", i, buffer);

        i++;
    }

    return NULL;
}

void fifo_to_stdout_thread_args_destroy(fifo_to_stdout_thread_args_t *args){
    free(args);
}
#endif