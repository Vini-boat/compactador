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
#include "threads/writer_thread.h"
#include "threads/thread_info.h"

void run_test_mode(args_t *args){
    thread_info_t reader_thread;
    thread_info_t writer_thread;

    fifo_t fifo_read_to_stdout;
    fifo_init(&fifo_read_to_stdout);

    reader_thread.args = (void *) reader_thread_args_create(args->to_compress_filename, &fifo_read_to_stdout);
    
    writer_thread.args = (void *) writer_thread_args_create(strcat(args->to_compress_filename,".cz"),&fifo_read_to_stdout);

    reader_thread.error = pthread_create(&reader_thread.id, NULL, run_reader_thread, reader_thread.args);
    if(reader_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando reader_thread %d\n",reader_thread.error);
        reader_thread_args_destroy((reader_thread_args_t *) reader_thread.args);
        writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
        fifo_destroy(&fifo_read_to_stdout);
        
        return;
    }
    printf("after:\n");
    printf("reader_thread:\n");
    print_thread_info(&reader_thread, "\t");
    
    writer_thread.error = pthread_create(&writer_thread.id, NULL, run_writer_thread,writer_thread.args);    
    if(writer_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando writer_thread %d\n",writer_thread.error);
        pthread_join(reader_thread.id, NULL);
        reader_thread_args_destroy(reader_thread.args);
        writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
        fifo_destroy(&fifo_read_to_stdout);
        return;
    }
    printf("writer_thread:\n");
    print_thread_info(&writer_thread, "\t");
    
    pthread_join(reader_thread.id, &reader_thread.status);
    pthread_join(writer_thread.id, &writer_thread.status);
    
    if (reader_thread.status != NULL || writer_thread.status != NULL) {
        fprintf(stderr, "[ERROR] Uma das threads falhou durante a execução.\n");
    }
    printf("after:\n");
    printf("reader_thread:\n");
    print_thread_info(&reader_thread, "\t");
    printf("writer_thread:\n");
    print_thread_info(&writer_thread, "\t");
    
    writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
    reader_thread_args_destroy((reader_thread_args_t *) reader_thread.args);
    
    fifo_destroy(&fifo_read_to_stdout);


}
#endif