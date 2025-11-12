#ifndef STATS_SHM_H
#define STATS_SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <string.h>

typedef struct {
    long read_tokens_total;
    long compressed_tokens_total;
    long tokens_not_in_dict_total;
    long long original_size_bytes;
} compressor_stats_data_t;

typedef struct {
    pthread_mutex_t mutex;
    compressor_stats_data_t data;
} compressor_stats_t;

typedef struct {
    long read_bytes_total;
    long decompressed_bytes_total;
    long bytes_not_in_dict_total;
    long long final_size_bytes;
} decompressor_stats_data_t;

typedef struct {
    pthread_mutex_t mutex;
    decompressor_stats_data_t data;
} decompressor_stats_t;

typedef struct {
    compressor_stats_t comp_stats;
    decompressor_stats_t decomp_stats;
} stats_shm_t;

typedef struct {
    compressor_stats_data_t comp_data;
    decompressor_stats_data_t decomp_data;
} stats_snap_t;

stats_shm_t* stats_shm_create();

void stats_shm_comp_update(stats_shm_t *stats, int read_tokens, int compressed_tokens, int tokens_not_in_dict);

void stats_shm_comp_set_original_size_bytes(stats_shm_t *stats, long long size);


void stats_shm_decomp_update(stats_shm_t *stats, int read_bytes, int decompressed_bytes, int bytes_not_in_dict);

void stats_shm_decomp_set_final_size_bytes(stats_shm_t *stats, long long size);


void stats_shm_get_snap(stats_shm_t *stats, stats_snap_t *snap_out);

void stats_shm_destroy(stats_shm_t *stats);



#endif