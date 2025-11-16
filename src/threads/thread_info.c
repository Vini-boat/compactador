#ifndef THREAD_INFO_C
#define THREAD_INFO_C
#include "threads/thread_info.h"
#include <pthread.h>
#include <stdio.h>

void print_thread_info(thread_info_t *info, char *prefix){
    printf("%s",prefix);
    printf("id     : %lu\n",info->id);
    printf("%s",prefix);
    printf("*args  : %p\n",info->args);
    printf("%s",prefix);
    printf("error  : %d\n",info->error);
    printf("%s",prefix);
    printf("*status: %p\n",info->status);
}
#endif