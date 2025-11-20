#ifndef DICTIONARY_C
#define DICTIONARY_C
#include "structures/dictionary.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void dict_add_default_values(dict_t *dict){
    char *words[] = {"este"," ","material","de","exemplo","foi","criado","para","avaliar","uma","compressao","simples","palavras","frequentes","em","portugues",".","\n","a","ideia","e","mostrar","como","um","programa","c","pode","trabalhar","com","processos","threads","ler","gravar","arquivos",",","formato","direto","facil","entender","quando","falamos","compactacao","texto","o","foco","aqui","logica","nao","taxa","maxima","ainda","assim","possivel","ganhar","muito","escolhemos","pequeno","dicionario","comuns","textos","academicos","ou","tecnicos","por","bastante","normal","ver","frases","muitas","ocorrencias","curtas","que","aparecem","varios","contextos","ate","mesmo","duas","combinacoes","semelhantes","objetivo","aluno","foque","no","uso",":","processo","filho","faz","pai","exibe","apenas","relatorio","final","estatisticas","leitura","do","cada","linha","contem","simbolo","palavra","encontra","correspondente","grava","arquivo","saida","caso","encontre","original","mais","longo","comum","os","ganhos","sejam","maiores","porque","as","vezes","se","tem","tantas","repeticoes","diferenca","ser","menor","mas","solucao","continua","util","aprendizado","inclui","varias","estimular","repetidas","fato","pensamos","escrever","estruturas","repetem","paragrafos","diferentes","isso","ajuda","metodo","tambem","observar","deve","preservar","espacos","pontuacao","descompactado","seja","exatamente","igual","ao","concluir","preparado","testes","da","atividade","ele","organizado","modo","conter","muitos","termos","linhas","O","demonstra","reduz","tamanho","utiliza","trabalha","mostra","possui","repete","fica","Este","efeito","sobre","criados","trabalham","juntos","comprimir","eficiente","Assim","comprova"};
    size_t words_n = sizeof(words) / sizeof(words[0]);

    unsigned char current_code = 0xF;

    for(size_t i = 0; i < words_n; i++){
        dict_add_item(dict,current_code++,words[i]);
    }
}

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