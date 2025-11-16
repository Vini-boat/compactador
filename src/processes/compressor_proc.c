#ifndef COMPRESSOR_PROC_C
#define COMPRESSOR_PROC_C

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "processes/compressor_proc.h"
#include "structures/stats_shm.h"
#include "args.h"

#include "structures/fifo_thread_safe.h"

#include "threads/thread_info.h"
#include "threads/token_reader_thread.h"
#include "threads/compressor_thread.h"
#include "threads/writer_thread.h"

void run_compressor_proc(args_t *args, stats_shm_t *stats){
    thread_info_t token_reader_thread;
    thread_info_t compressor_thread;
    thread_info_t writer_thread;

    fifo_t fifo_read_to_compressor;
    fifo_init(&fifo_read_to_compressor);

    fifo_t fifo_compressor_to_writer;
    fifo_init(&fifo_compressor_to_writer);

    token_reader_thread.args = (void *) token_reader_thread_args_create(args->to_compress_filename, &fifo_read_to_compressor);
    
    compressor_thread.args = (void *) compressor_thread_args_create(&fifo_read_to_compressor,&fifo_compressor_to_writer);
 
    writer_thread.args = (void *) writer_thread_args_create(strcat(args->to_compress_filename,".cz"),&fifo_compressor_to_writer);

    token_reader_thread.error = pthread_create(&token_reader_thread.id, NULL, run_token_reader_thread, token_reader_thread.args);
    if(token_reader_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando token_reader_thread %d\n",token_reader_thread.error);
        
        token_reader_thread_args_destroy((token_reader_thread_args_t *) token_reader_thread.args);
        compressor_thread_args_destroy((compressor_thread_args_t *) compressor_thread.args);
        writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
        
        fifo_destroy(&fifo_read_to_compressor);
        fifo_destroy(&fifo_compressor_to_writer);
        
        return;
    }
    
    compressor_thread.error = pthread_create(&compressor_thread.id, NULL, run_compressor_thread,compressor_thread.args);    
    if(compressor_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando compressor_thread %d\n",compressor_thread.error);
        
        pthread_join(token_reader_thread.id, NULL);
        
        token_reader_thread_args_destroy((token_reader_thread_args_t *) token_reader_thread.args);
        compressor_thread_args_destroy((compressor_thread_args_t *) compressor_thread.args);
        writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
        
        fifo_destroy(&fifo_read_to_compressor);
        fifo_destroy(&fifo_compressor_to_writer);
        
        return;
    }

    writer_thread.error = pthread_create(&writer_thread.id, NULL, run_writer_thread,writer_thread.args);    
    if(writer_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando writer_thread %d\n",writer_thread.error);
        
        pthread_join(token_reader_thread.id, NULL);
        pthread_join(compressor_thread.id, NULL);
        
        token_reader_thread_args_destroy((token_reader_thread_args_t *) token_reader_thread.args);
        compressor_thread_args_destroy((compressor_thread_args_t *) compressor_thread.args);
        writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
        
        fifo_destroy(&fifo_read_to_compressor);
        fifo_destroy(&fifo_compressor_to_writer);
        return;
    }

    pthread_join(token_reader_thread.id, &token_reader_thread.status);
    pthread_join(compressor_thread.id, &compressor_thread.status);
    pthread_join(writer_thread.id, &writer_thread.status);

    
    if (token_reader_thread.status != NULL || compressor_thread.status != NULL || writer_thread.status != NULL) {
        fprintf(stderr, "[ERROR] Uma das threads falhou durante a execução.\n");
    }
    
    writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
    compressor_thread_args_destroy((compressor_thread_args_t *) compressor_thread.args);
    token_reader_thread_args_destroy((token_reader_thread_args_t *) token_reader_thread.args);
    
    fifo_destroy(&fifo_read_to_compressor);
    fifo_destroy(&fifo_compressor_to_writer);
}

#endif