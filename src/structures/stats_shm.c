#ifndef STATS_SHM_C
#define STATS_SHM_C
#include "structures/stats_shm.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>

#define SHM_PROTECTION (PROT_READ | PROT_WRITE)
#define SHM_FLAGS (MAP_SHARED | MAP_ANONYMOUS)
#define SHM_FD -1
#define SHM_OFFSET 0

stats_shm_t* stats_shm_create(){
    stats_shm_t *stats = mmap(NULL, sizeof(stats_shm_t), SHM_PROTECTION, SHM_FLAGS, SHM_FD, 0);
    if(stats == MAP_FAILED){
        perror("[ERROR] Alocação de memória compartilhada falhou");
        return NULL;
    }
    
    memset(stats, 0, sizeof(stats_shm_t));

    pthread_mutexattr_t attr;
    if( pthread_mutexattr_init(&attr) != 0){
        perror("[ERROR] pthread_mutexattr_init");
        munmap(stats, sizeof(stats_shm_t));
        return NULL;
    }
    
    if( pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0){
        perror("[ERROR] pthread_mutexattr_setpshared");
        pthread_mutexattr_destroy(&attr);
        munmap(stats, sizeof(stats_shm_t));
        return NULL;
    }
    
    if( pthread_mutex_init(&stats->comp_stats.mutex, &attr) != 0){
        perror("[ERROR] pthread_mutex_init stats->comp_stats.mutex");
        pthread_mutexattr_destroy(&attr);
        munmap(stats, sizeof(stats_shm_t));
        return NULL;
    }
    
    if( pthread_mutex_init(&stats->decomp_stats.mutex, &attr) != 0){
        perror("[ERROR] pthread_mutex_init stats->decomp_stats.mutex");
        pthread_mutexattr_destroy(&attr);
        pthread_mutex_destroy(&stats->comp_stats.mutex);
        munmap(stats, sizeof(stats_shm_t));
        return NULL;
    }

    pthread_mutexattr_destroy(&attr);
    
    return stats;
}

void stats_shm_destroy(stats_shm_t *stats){
    if(stats == NULL) return;

    pthread_mutex_destroy(&stats->comp_stats.mutex);
    pthread_mutex_destroy(&stats->decomp_stats.mutex);

    if( munmap(stats, sizeof(stats_shm_t)) == -1){
        perror("[ERROR] Desanexar memória copartilhada");
    }
}

#endif