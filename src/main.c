#include <stdio.h>
#include <stdlib.h>
#include "structures/fifo.h"

//int main(int argc, char *argv[]) {
int main() {

    fifo_t fila;
    fifo_init(&fila);
    
    fifo_push(&fila, "teste1");
    fifo_push(&fila, "teste2");
    
    char buffer[MAX_WORD_LENGTH];
    
    fifo_pop(&fila, buffer);
    printf("teste: %s\n", buffer);
    
    fifo_push(&fila, "teste3");
    fifo_pop(&fila, buffer);
    printf("teste: %s\n", buffer);
    fifo_pop(&fila, buffer);
    printf("teste: %s\n", buffer);

    fifo_destroy(&fila);
}