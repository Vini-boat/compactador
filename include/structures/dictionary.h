#ifndef DICTIONARY_H
#define DICTIONARY_H
#include "structures/fifo_thread_safe.h"
#define DICT_MAX_TOKEN_LENGTH FIFO_MAX_WORD_LENGTH 
#define DICT_INITIAL_SIZE 256


typedef struct {
    char code;
    char token[DICT_MAX_TOKEN_LENGTH];

} dict_item_t;

typedef struct {
    dict_item_t *items;
    int count;
    int capacity;
} dict_t;

dict_t* dict_create();

void dict_add_item(dict_t *dict, char code, char *token);

int dict_get_code(dict_t *dict, char *token, char *code_out);

int dict_get_token(dict_t *dict, char code, char *token_out);

void dict_destroy(dict_t *dict);

#endif