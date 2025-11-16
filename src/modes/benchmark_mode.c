#ifndef BENCHMARK_MODE_C
#define BENCHMARK_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/benchmark_mode.h"

void run_benchmark_mode(args_t *args){

    // só para parar o -Wunused-parameter enquando não colocamos nada aqui
    char * filename = args->to_compress_filename;
    filename++;
    // só para parar o -Wunused-parameter enquando não colocamos nada aqui

    pid_t pid_compressor;
    pid_t pid_decompressor;
    pid_t pid_monitor;

    pid_compressor = fork();
    if(pid_compressor == 0){
        //run compactador
        exit(0);
    }
    pid_decompressor = fork();
    if(pid_decompressor == 0){
        //run descompactador
        exit(0);
    }
    pid_monitor = fork();
    if(pid_monitor == 0){
        //run monitor
        exit(0);
    }


    int status;
    waitpid(pid_compressor,&status,0);
    waitpid(pid_decompressor,&status,0);
    waitpid(pid_monitor,&status,0);
}

#endif