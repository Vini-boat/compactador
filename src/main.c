#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "args.h"

int main(int argc, char *argv[]) {

    args_t args;
    args_parse_params(argc,argv,&args);

    pid_t pid_compactador;
    pid_t pid_descompactador;
    pid_t pid_monitor;

    pid_compactador = fork();
    if(pid_compactador == 0){
        //run compactador
        exit(0);
    }
    pid_descompactador = fork();
    if(pid_descompactador == 0){
        //run descompactador
        exit(0);
    }
    pid_monitor = fork();
    if(pid_monitor == 0){
        //run monitor
        exit(0);
    }


    int status;
    waitpid(pid_compactador,&status,0);
    waitpid(pid_descompactador,&status,0);
    waitpid(pid_monitor,&status,0);

    return 0;
}