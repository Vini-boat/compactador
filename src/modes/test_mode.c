#ifndef TEST_MODE_C
#define TEST_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/test_mode.h"
#include "structures/fifo_thread_safe.h"
#include "structures/stats_shm.h"
#include "processes/monitor_proc.h"
#include "sys/stat.h"


void run_test_mode(args_t *args){
    pid_t pid_monitor;
    
    struct stat st;
    stats_shm_t *stats = stats_shm_create();
    if(stat(args->to_compress_filename,&st) == 0){
        stats_shm_comp_set_original_size_bytes(stats,(long long) st.st_size);
    }
    pid_monitor = fork();
    if(pid_monitor == 0){
        run_monitor_proc(args,stats);
        exit(0);
    }

    for(int i =0; i<15;i++){
        sleep(1);
        stats_shm_comp_update(stats,10,20,30);
    }
    stats_shm_comp_set_finished(stats,1);
    for(int i =0; i<15;i++){
        sleep(1);
        stats_shm_decomp_update(stats,10,20,30);
    }
    stats_shm_decomp_set_finished(stats,1);

    
    int status;
    waitpid(pid_monitor,&status,0);
    
    stats_shm_destroy(stats);
}
#endif