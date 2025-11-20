#ifndef THREAD_INFO_H
#define THREAD_INFO_H
#include <pthread.h>

typedef struct {
    pthread_t id;
    void *args;
    int error;
    void *status;
} thread_info_t;

void thread_info_init(thread_info_t *info);

void print_thread_info(thread_info_t *info, char *prefix);
#endif