#ifndef COMPRESSOR_PROC_C
#define COMPRESSOR_PROC_C
#include "threads/compressor_thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures/fifo_thread_safe.h"
#include "structures/dictionary.h"

#define COMP_BEGIN_FOOTER_CODE 0x1
#define COMP_INITIAL_CODE 0x2
#define COMP_HEADER_ITEM_LENGH (FIFO_MAX_WORD_LENGTH + 2)

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
    char current_code = COMP_INITIAL_CODE;

    char token_buffer[FIFO_MAX_WORD_LENGTH];
    char code_string[2];
    code_string[0] = '#';
    code_string[1] = '\0';

    while(1){
        fifo_pop(args->fifo_to_read, token_buffer);
        if(strcmp(token_buffer,FIFO_FINAL_TOKEN) == 0) break;

        if(dict_get_code(dict,token_buffer, &code_string[0]) == -1){
            dict_add_item(dict,current_code,token_buffer);
            code_string[0] = current_code;
            current_code++;
        }

        fifo_push(args->fifo_to_write,code_string);
    }
    
    code_string[0] = COMP_BEGIN_FOOTER_CODE;
    fifo_push(args->fifo_to_write,code_string);

    for(int i =0; i<dict->count; i++){
        code_string[0] = dict->items[i].code;
        fifo_push(args->fifo_to_write,code_string);
        fifo_push(args->fifo_to_write,"dict_add_item(dict,0x0,\"\0");
        fifo_push(args->fifo_to_write,dict->items[i].token);
        fifo_push(args->fifo_to_write,"\");\0");
        fifo_push(args->fifo_to_write,"\n\0");
    }
    printf("%d\n",dict->count);
    fifo_push(args->fifo_to_write, FIFO_FINAL_TOKEN);

    dict_destroy(dict);
    return NULL;
}

void compressor_thread_args_destroy(compressor_thread_args_t *args){
    free(args);
}


#endif