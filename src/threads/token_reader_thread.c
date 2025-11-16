#ifndef TOKEN_READER_THREAD_C
#define TOKEN_READER_THREAD_C
#include "threads/token_reader_thread.h"
#include "structures/fifo_thread_safe.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

token_reader_thread_args_t* token_reader_thread_args_create(const char* filename, fifo_t* fifo_to_write){
    token_reader_thread_args_t *args = (token_reader_thread_args_t*) malloc(sizeof(token_reader_thread_args_t));
    if (args == NULL){
        perror("[ERROR] malloc em token_reader_thread_args_create");
        return NULL;
    }

    args->filename = strdup(filename); 
    if (args->filename == NULL) {
        perror("[ERROR] strdup em token_reader_thread_args_create");
        free(args);
        return NULL;
    }


    args->fifo_to_write = fifo_to_write;

    return args;
}

void* run_token_reader_thread(void *token_reader_thread_args){
    token_reader_thread_args_t *args = (token_reader_thread_args_t*) token_reader_thread_args;

    FILE *file = fopen(args->filename, "r");
    
    char token[FIFO_MAX_WORD_LENGTH];
    int token_index = 0;
    int c;

    do {
        c = fgetc(file);

        if (isspace(c) || ispunct(c) || c == EOF){
            if(token_index > 0){
                token[token_index] = '\0';
                fifo_push(args->fifo_to_write, token);
                token_index = 0;
            }
            if (c == EOF) continue;
            
            token_index = 0;
            token[token_index++] = (char)c;
            token[token_index] = '\0';
            fifo_push(args->fifo_to_write, token);
            token_index = 0;
        } else {
            if(token_index == FIFO_MAX_WORD_LENGTH -1){
                token[token_index] = '\0';
                fifo_push(args->fifo_to_write, token);
                token_index = 0;
            }

            token[token_index++] = (char)c;
            continue;
        }

    } while (c != EOF);

    fclose(file);
    fifo_push(args->fifo_to_write, FIFO_FINAL_TOKEN);

    return NULL;
}

void token_reader_thread_args_destroy(token_reader_thread_args_t *args){
    free(args->filename);
    free(args);
}

#endif