#ifndef DECOMPRESSION_MODE_C
#define DECOMPRESSION_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/decompression_mode.h"

void run_decompression_mode(args_t *args){

    // só para parar o -Wunused-parameter enquando não colocamos nada aqui
    char * filename = args->to_decompress_filename;
    filename++;
    // só para parar o -Wunused-parameter enquando não colocamos nada aqui

    pid_t pid_decompressor;
    pid_t pid_monitor;

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
    waitpid(pid_decompressor,&status,0);
    waitpid(pid_monitor,&status,0);
}
#endif