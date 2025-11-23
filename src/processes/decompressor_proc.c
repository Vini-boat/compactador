#ifndef DECOMPRESSOR_PROC_C
#define DECOMPRESSOR_PROC_C

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "processes/decompressor_proc.h"
#include "structures/stats_shm.h"
#include "args.h"

#include "structures/fifo_thread_safe.h"

#include "threads/thread_info.h"
#include "threads/code_reader_thread.h"
#include "threads/decompressor_thread.h"
#include "threads/writer_thread.h"

#include "sys/stat.h"

void run_decompressor_proc(args_t *args, stats_shm_t *stats){
    thread_info_t code_reader_thread;
    thread_info_init(&code_reader_thread);
    thread_info_t decompressor_thread;
    thread_info_init(&decompressor_thread);
    thread_info_t writer_thread;
    thread_info_init(&writer_thread);

    fifo_t fifo_read_to_decompressor;
    fifo_init(&fifo_read_to_decompressor);

    fifo_t fifo_decompressor_to_writer;
    fifo_init(&fifo_decompressor_to_writer);

    code_reader_thread.args = (void *) code_reader_thread_args_create(args->to_decompress_filename, &fifo_read_to_decompressor);

    decompressor_thread.args = (void *) decompressor_thread_args_create(stats,&fifo_read_to_decompressor,&fifo_decompressor_to_writer);

    //remove a extenção 
    args->to_decompress_filename[strlen(args->to_decompress_filename) - strlen(".cz")] = '\0';  
    if(args->mode == 'b'){
        args->to_decompress_filename[strlen(args->to_decompress_filename) - strlen(".txt")] = '\0';
        strcat(args->to_decompress_filename,"-b.txt");  
    }
    writer_thread.args = (void *) writer_thread_args_create(args->to_decompress_filename,&fifo_decompressor_to_writer);

    code_reader_thread.error = pthread_create(&code_reader_thread.id, NULL, run_code_reader_thread, code_reader_thread.args);
    if(code_reader_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando code_reader_thread %d\n",code_reader_thread.error);
        goto cleanup_error;
    }
    
    decompressor_thread.error = pthread_create(&decompressor_thread.id, NULL, run_decompressor_thread,decompressor_thread.args);    
    if(decompressor_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando decompressor_thread %d\n",decompressor_thread.error);
        goto cleanup_error;
    }

    writer_thread.error = pthread_create(&writer_thread.id, NULL, run_writer_thread,writer_thread.args);    
    if(writer_thread.error != 0) {
        fprintf(stderr, "[ERROR] Criando writer_thread %d\n",writer_thread.error);
        goto cleanup_error;
    }

    pthread_join(code_reader_thread.id, &code_reader_thread.status);
    pthread_join(decompressor_thread.id, &decompressor_thread.status);
    pthread_join(writer_thread.id, &writer_thread.status);

    struct stat st;
    if(stat(args->to_decompress_filename,&st) == 0){
        stats_shm_decomp_set_final_size_bytes(stats,(long long) st.st_size);
    }

    stats_shm_decomp_set_finished(stats,1);
    
    if (code_reader_thread.status != NULL || decompressor_thread.status != NULL || writer_thread.status != NULL) {
        fprintf(stderr, "[ERROR] Uma das threads falhou durante a execução.\n");
    }


    goto cleanup_resources;

    cleanup_error:
        if (code_reader_thread.error == 0) {
            pthread_cancel(code_reader_thread.id);
            pthread_join(code_reader_thread.id, NULL);
        }
        if (decompressor_thread.error == 0) {
            pthread_cancel(decompressor_thread.id);
            pthread_join(decompressor_thread.id, NULL);
        }
        if (writer_thread.error == 0) {
            pthread_cancel(writer_thread.id);
            pthread_join(writer_thread.id, NULL);
        }
    cleanup_resources:
        if (code_reader_thread.args) code_reader_thread_args_destroy((code_reader_thread_args_t *) code_reader_thread.args);
        if (decompressor_thread.args) decompressor_thread_args_destroy((decompressor_thread_args_t *) decompressor_thread.args);
        if (writer_thread.args) writer_thread_args_destroy((writer_thread_args_t *) writer_thread.args);
        
        fifo_destroy(&fifo_read_to_decompressor);
        fifo_destroy(&fifo_decompressor_to_writer);

}

#endif