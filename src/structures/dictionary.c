#ifndef DICTIONARY_C
#define DICTIONARY_C
#include "structures/dictionary.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void _dict_double_capacity(dict_t *dict){
    int new_capacity = dict->capacity * 2;
    size_t new_size =  sizeof(dict_item_t) * new_capacity;

    dict_item_t *new_items = realloc(dict->items,new_size);
    if (new_items == NULL){
        perror("[ERROR] realoc em _dict_double_capacity");
        exit(1);
    }

    dict->items = new_items;
    dict->capacity = new_capacity; 
}

dict_t* dict_create(){
    dict_t *dict = (dict_t *) malloc(sizeof(dict_t));
    if (dict == NULL){
        perror("[ERROR] malloc dict_t * em dict_create");
        return NULL;
    }
    
    dict->count = 0;
    dict->capacity = DICT_INITIAL_SIZE;
    dict->items = (dict_item_t *) malloc(sizeof(dict_item_t) * DICT_INITIAL_SIZE);
    if (dict->items == NULL){
        perror("[ERROR] malloc dict_item_t * em dict_create");
        free(dict);
        return NULL;
    }
    
    return dict;
}

void dict_add_item(dict_t *dict, char code, char *token){
    dict_item_t item;
    item.code = code;
    strcpy(item.token, token);

    if(dict->count == dict->capacity){
        _dict_double_capacity(dict);
    }

    dict->items[dict->count] = item;
    dict->count++;
}

int dict_get_code(dict_t *dict, char *token, char *code_out){
    for(int i =0; i<dict->count; i++){
        if(strcmp(dict->items[i].token, token) == 0){
            *code_out = dict->items[i].code;
            return 0;
        }
    }
    return -1;
}

int dict_get_token(dict_t *dict, char code, char *token_out){
    for(int i =0; i<dict->count; i++){
        if(dict->items[i].code == code){
            strcpy(token_out,dict->items[i].token);
            return 0;
        }
    }
    return -1;
}

void dict_destroy(dict_t *dict){
    if (dict == NULL) return;
    free(dict->items);
    free(dict);
}

#endif