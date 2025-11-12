#ifndef STATS_SHM_H
#define STATS_SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>

typedef struct {
    pthread_mutex_t mutex;

    long read_tokens_total;
    long compressed_tokens_total;
    long tokens_not_in_dict_total;
    long long original_size_bytes;

} compressor_stats_t;

typedef struct {
    pthread_mutex_t mutex;

    long read_bytes_total;
    long decmpressed_tokens_total;
    long tokens_not_in_dict_total;
    long long final_size_bytes;

} decompressor_stats_t;

typedef struct {
    compressor_stats_t comp_stats;
    decompressor_stats_t decomp_stats;
} stats_shm_t;

stats_shm_t* stats_shm_create();

void stats_shm_destroy(stats_shm_t *stats);



#endif