#ifndef ARGS_H
#define ARGS_H
#include <stdio.h>
#include <stdlib.h> // Para exit()
#include <unistd.h> // Para getopt()

typedef struct{
    char mode;
    int show_monitor;
    char *to_compress_filename;
    char *to_decompress_filename;
} args_t;

void args_print_usage(const char *prog_name);

void args_exit_if_mode_already_set(args_t *args, const char *prog_name);

void args_exit_if_file_dont_exists(char *filename);

void args_parse_params(int argc,char *argv[], args_t *args);



#endif