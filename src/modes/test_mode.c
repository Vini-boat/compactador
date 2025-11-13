#ifndef TEST_MODE_C
#define TEST_MODE_C
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "modes/test_mode.h"
#include "structures/fifo_thread_safe.h"

#include "threads/reader_thread.h"
#include "threads/fifo_to_stdout_thread.h"
#include "threads/thread_info.h"

void run_test_mode(args_t *args){
    thread_info_t reader_thread;
    thread_info_t stdout_thread;

    fifo_t fifo_read_to_stdout;
    fifo_init(&fifo_read_to_stdout);

    reader_thread.args = (void *) reader_thread_args_create(args->to_compress_filename, &fifo_read_to_stdout);
    stdout_thread.args = (void *) fifo_to_stdout_thread_args_create(&fifo_read_to_stdout);
    

    reader_thread.error = pthread_create(&reader_thread.id, NULL, run_reader_thread, reader_thread.args);
    if(reader_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando reader_thread %d\n",reader_thread.error);
        reader_thread_args_destroy((reader_thread_args_t *) reader_thread.args);
        fifo_to_stdout_thread_args_destroy((fifo_to_stdout_thread_args_t *) stdout_thread.args);
        fifo_destroy(&fifo_read_to_stdout);
        
        return;
    }
    printf("reader_thread:\n");
    print_thread_info(&reader_thread, "\t");
    
    stdout_thread.error = pthread_create(&stdout_thread.id, NULL, run_fifo_to_stdout_thread,stdout_thread.args);    
    if(stdout_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando fifo_to_stdout_thread %d\n",stdout_thread.error);
        
        pthread_join(reader_thread.id, NULL);
        
        reader_thread_args_destroy(reader_thread.args);
        fifo_to_stdout_thread_args_destroy((fifo_to_stdout_thread_args_t *) stdout_thread.args);
        fifo_destroy(&fifo_read_to_stdout);
        
        return;
    }
    printf("stdout_thread:\n");
    print_thread_info(&stdout_thread, "\t");

    pthread_join(reader_thread.id, &reader_thread.status);
    pthread_join(stdout_thread.id, &stdout_thread.status);
    
    if (reader_thread.status != NULL || stdout_thread.status != NULL) {
        fprintf(stderr, "[ERROR] Uma das threads falhou durante a execução.\n");
    }

    fifo_to_stdout_thread_args_destroy((fifo_to_stdout_thread_args_t *) stdout_thread.args);
    reader_thread_args_destroy((reader_thread_args_t *) reader_thread.args);
    
    fifo_destroy(&fifo_read_to_stdout);
}
#endif