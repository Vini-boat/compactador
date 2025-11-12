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

// COMP

void stats_shm_comp_update(stats_shm_t *stats, int read_tokens, int compressed_tokens, int tokens_not_in_dict){
    pthread_mutex_lock(&stats->comp_stats.mutex);

    stats->comp_stats.data.read_tokens_total += read_tokens;
    stats->comp_stats.data.compressed_tokens_total += compressed_tokens;
    stats->comp_stats.data.tokens_not_in_dict_total += tokens_not_in_dict;

    pthread_mutex_unlock(&stats->comp_stats.mutex);
}

void stats_shm_comp_set_original_size_bytes(stats_shm_t *stats, long long size){
    pthread_mutex_lock(&stats->comp_stats.mutex);
    
    stats->comp_stats.data.original_size_bytes = size;
    
    pthread_mutex_unlock(&stats->comp_stats.mutex);
}

// DECOMP

void stats_shm_decomp_update(stats_shm_t *stats, int read_bytes, int decompressed_bytes, int bytes_not_in_dict){
    pthread_mutex_lock(&stats->decomp_stats.mutex);
    
    stats->decomp_stats.data.read_bytes_total += read_bytes;
    stats->decomp_stats.data.decompressed_bytes_total += decompressed_bytes;
    stats->decomp_stats.data.bytes_not_in_dict_total += bytes_not_in_dict;
    
    pthread_mutex_unlock(&stats->decomp_stats.mutex);
}

void stats_shm_decomp_set_final_size_bytes(stats_shm_t *stats, long long size){
    pthread_mutex_lock(&stats->decomp_stats.mutex);

    stats->decomp_stats.data.final_size_bytes = size;

    pthread_mutex_unlock(&stats->decomp_stats.mutex);
}

void stats_shm_get_snap(stats_shm_t *stats, stats_snap_t *snap_out){
    pthread_mutex_lock(&stats->comp_stats.mutex);
    snap_out->comp_data.read_tokens_total = stats->comp_stats.data.read_tokens_total;
    snap_out->comp_data.compressed_tokens_total = stats->comp_stats.data.compressed_tokens_total;
    snap_out->comp_data.tokens_not_in_dict_total = stats->comp_stats.data.tokens_not_in_dict_total;
    snap_out->comp_data.original_size_bytes = stats->comp_stats.data.original_size_bytes;
    pthread_mutex_unlock(&stats->comp_stats.mutex);
    
    pthread_mutex_lock(&stats->decomp_stats.mutex);
    snap_out->decomp_data.read_bytes_total = stats->decomp_stats.data.read_bytes_total;
    snap_out->decomp_data.decompressed_bytes_total = stats->decomp_stats.data.decompressed_bytes_total;
    snap_out->decomp_data.bytes_not_in_dict_total = stats->decomp_stats.data.bytes_not_in_dict_total;
    snap_out->decomp_data.final_size_bytes = stats->decomp_stats.data.final_size_bytes;
    pthread_mutex_unlock(&stats->decomp_stats.mutex);

}

#endif