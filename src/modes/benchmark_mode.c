#ifndef BENCHMARK_MODE_C
#define BENCHMARK_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/benchmark_mode.h"

#include "processes/compressor_proc.h"
#include "processes/decompressor_proc.h"

#include "structures/stats_shm.h"

void run_benchmark_mode(args_t *args){
    pid_t pid_compressor;
    pid_t pid_decompressor;
    pid_t pid_monitor;

    stats_shm_t *stats = stats_shm_create();

    args->to_decompress_filename = strdup(args->to_compress_filename);
    strcat(args->to_decompress_filename,".cz");



    pid_compressor = fork();
    if(pid_compressor == 0){
        run_compressor_proc(args,stats);
        exit(0);
    }
    pid_monitor = fork();
    if(pid_monitor == 0){
        //run monitor
        exit(0);
    }
    int status;
    waitpid(pid_compressor,&status,0);
    
    pid_decompressor = fork();
    if(pid_decompressor == 0){
        run_decompressor_proc(args,stats);
        exit(0);
    }
    
    waitpid(pid_decompressor,&status,0);
    waitpid(pid_monitor,&status,0);
    stats_shm_destroy(stats);
}

#endif