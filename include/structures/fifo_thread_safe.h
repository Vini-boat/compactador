#ifndef FIFO_H
#define FIFO_H
#include <pthread.h>
#include <string.h>

// TODO: revisar e ajustar esses valores
#define FIFO_CAPACITY 100
#define FIFO_MAX_WORD_LENGTH 128
#define FIFO_FINAL_TOKEN ""

typedef struct {

    char buffer[FIFO_CAPACITY][FIFO_MAX_WORD_LENGTH];

    int head;
    int tail;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t cond_nao_cheia;
    pthread_cond_t cond_nao_vazia;

} fifo_t;

void fifo_init(fifo_t *fifo);

void fifo_push(fifo_t *fifo, const char *buffer_in);

void fifo_pop(fifo_t *fifo, char *buffer_out);

void fifo_drain(fifo_t *fifo);

void fifo_destroy(fifo_t *fifo);

#endif
