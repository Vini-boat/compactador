#ifndef DECOMPRESSION_MODE_C
#define DECOMPRESSION_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/decompression_mode.h"
#include "processes/decompressor_proc.h"

#include "structures/stats_shm.h"

void run_decompression_mode(args_t *args){

    pid_t pid_decompressor;
    pid_t pid_monitor;

    stats_shm_t *stats = stats_shm_create();

    pid_decompressor = fork();
    if(pid_decompressor == 0){
        run_decompressor_proc(args,stats);
        exit(0);
    }
    if(args->show_monitor == 1){
        pid_monitor = fork();
        if(pid_monitor == 0){
            // run monitor
            exit(0);
        }
    }


    int status;
    waitpid(pid_decompressor,&status,0);
    if(args->show_monitor == 1){
        waitpid(pid_monitor,&status,0);
    }
    stats_shm_destroy(stats);
}
#endif