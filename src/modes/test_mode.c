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

void run_test_mode(args_t *args){
    pthread_t reader_thread_id;
    pthread_t fifo_to_stdout_thread_id;

    fifo_t fifo_read_to_stdout;
    fifo_init(&fifo_read_to_stdout);

    reader_thread_args_t* reader_thread_args = reader_thread_args_create(args->to_compress_filename, &fifo_read_to_stdout);
    fifo_to_stdout_thread_args_t* fifo_to_stdout_thread_args = fifo_to_stdout_thread_args_create(&fifo_read_to_stdout);
    

    int reader_thread_error = pthread_create(&reader_thread_id, NULL, run_reader_thread, (void*) reader_thread_args);

    if(reader_thread_error != 0) {
        fprintf(stderr, "[ERROR] Criando reader_thread %d\n",reader_thread_error);
        reader_thread_args_destroy(reader_thread_args);
        fifo_to_stdout_thread_args_destroy(fifo_to_stdout_thread_args);
        fifo_destroy(&fifo_read_to_stdout);
        
        return;
    }
    
    int fifo_to_stdout_thread_error = pthread_create(&fifo_to_stdout_thread_id, NULL, run_fifo_to_stdout_thread, (void*) fifo_to_stdout_thread_args);
    
    if(fifo_to_stdout_thread_error != 0) {
        fprintf(stderr, "[ERROR] Criando fifo_to_stdout_thread %d\n",fifo_to_stdout_thread_error);

        pthread_join(reader_thread_id, NULL);

        reader_thread_args_destroy(reader_thread_args);
        fifo_to_stdout_thread_args_destroy(fifo_to_stdout_thread_args);
        fifo_destroy(&fifo_read_to_stdout);
        
        return;
    }

    void *reader_status;
    void *stdout_status;

    pthread_join(reader_thread_id, &reader_status);
    pthread_join(fifo_to_stdout_thread_id, &stdout_status);
    
    if (reader_status != NULL || stdout_status != NULL) {
        fprintf(stderr, "[ERROR] Uma das threads falhou durante a execução.\n");
    }

    fifo_to_stdout_thread_args_destroy(fifo_to_stdout_thread_args);
    reader_thread_args_destroy(reader_thread_args);
    
    fifo_destroy(&fifo_read_to_stdout);
}
#endif