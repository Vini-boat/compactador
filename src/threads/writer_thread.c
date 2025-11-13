#ifndef WRITER_THREAD_C
#define WRITER_THREAD_C
#include "threads/writer_thread.h"
#include "structures/fifo_thread_safe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

writer_thread_args_t* writer_thread_args_create(const char* filename, fifo_t* fifo_to_read){
    writer_thread_args_t *args = (writer_thread_args_t*) malloc(sizeof(writer_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em writer_thread_args_create");
        return NULL;
    }

    args->filename = strdup(filename); 
    if (args->filename == NULL) {
        perror("[ERROR] strdup em writer_thread_args_create");
        free(args);
        return NULL;
    }

    args->fifo_to_read = fifo_to_read;
    
    return args;
}

void* run_writer_thread(void *writer_thread_args){
    writer_thread_args_t *args = (writer_thread_args_t*) writer_thread_args;
    FILE *file = fopen(args->filename, "w");
    if (file == NULL){
        perror("[ERROR] Writer thread não conseguiu abrir o arquivo");
        fifo_drain(args->fifo_to_read);
        return (void *) -1;
    }

    char buffer[FIFO_MAX_WORD_LENGTH];

    while(1){
        fifo_pop(args->fifo_to_read, buffer);
        if(strcmp(buffer, FIFO_FINAL_TOKEN) == 0) break;
        fputs(buffer,file);
    }

    fclose(file);
    return NULL;
}

void writer_thread_args_destroy(writer_thread_args_t *args){
    free(args->filename);
    free(args);
}

#endif