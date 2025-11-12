#ifndef TEST_MODE_C
#define TEST_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/test_mode.h"

void run_test_mode(args_t *args){
    // só para parar o -Wunused-parameter enquando não colocamos nada aqui
    char * filename = args->to_compress_filename;
    filename++;
    // só para parar o -Wunused-parameter enquando não colocamos nada aqui
}
#endif