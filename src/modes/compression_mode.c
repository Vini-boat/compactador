#ifndef COMPRESSION_MODE_C
#define COMPRESSION_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/compression_mode.h"
#include "processes/compressor_proc.h"

#include "structures/stats_shm.h"

void run_compression_mode(args_t *args){

    pid_t pid_compressor;
    pid_t pid_monitor;

    stats_shm_t *stats = stats_shm_create();

    pid_compressor = fork();
    if(pid_compressor == 0){
        run_compressor_proc(args,stats);
        exit(0);
    }
    if(args->show_monitor == 1){
        pid_monitor = fork();
        if(pid_monitor == 0){
            //run monitor
            exit(0);
        }
    }


    int status;
    waitpid(pid_compressor,&status,0);
    
    if(args->show_monitor == 1){
        waitpid(pid_monitor,&status,0);
    }

    stats_shm_destroy(stats);
}

#endif