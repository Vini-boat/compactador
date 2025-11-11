#ifndef FIFO_C
#define FIFO_C

#include "structures/fifo.h"
#include <pthread.h>

void fifo_init(fifo_t *fifo){
    memset(fifo,0,sizeof(fifo_t)); // zera a memória do struct para evitar lixo

    fifo->head = 0;
    fifo->tail = 0;
    fifo->count = 0;
    
    pthread_mutex_init(&fifo->mutex, NULL);
    pthread_cond_init(&fifo->cond_nao_cheia, NULL);
    pthread_cond_init(&fifo->cond_nao_vazia, NULL);
}

void fifo_destroy(fifo_t *fifo){
    pthread_mutex_destroy(&fifo->mutex);
    pthread_cond_destroy(&fifo->cond_nao_cheia);
    pthread_cond_destroy(&fifo->cond_nao_vazia);
}

void fifo_push(fifo_t *fifo, const char *buffer_in){
    pthread_mutex_lock(&fifo->mutex);
    // Libera o mutex até a fila não estar cheia
    while(fifo->count == FIFO_CAPACITY){
        pthread_cond_wait(&fifo->cond_nao_cheia, &fifo->mutex);
    }

    strcpy(fifo->buffer[fifo->head],buffer_in);
    fifo->head = (fifo->head + 1) % FIFO_CAPACITY;
    fifo->count++;

    // Avisa que agora tem pelo menos 1 item na fila
    pthread_cond_signal(&fifo->cond_nao_vazia);
    pthread_mutex_unlock(&fifo->mutex);
}

void fifo_pop(fifo_t *fifo, char *buffer_out){
    pthread_mutex_lock(&fifo->mutex);
    
    while(fifo->count == 0){
        pthread_cond_wait(&fifo->cond_nao_vazia, &fifo->mutex);
    }

    strcpy(buffer_out,fifo->buffer[fifo->tail]);
    fifo->tail = (fifo->tail + 1) % FIFO_CAPACITY;
    fifo->count--;

    pthread_cond_signal(&fifo->cond_nao_cheia);
    pthread_mutex_unlock(&fifo->mutex);
}

#endif