#ifndef ARGS_C
#define ARGS_C
#include <stdio.h>
#include <stdlib.h> // Para exit()
#include <unistd.h> // Para getopt()
#include "args.h"

// O VSCODE ACHA QUE O OPTARG E O OPTIND NÃO EXISTEM
//   SÓ POR ISSO QUE ELE RECLAMA NO INTELISENSE

void args_print_usage(const char *prog_name){
    fprintf(stderr, "Uso: %s [-m] [ -c <arquivo>] [ -d <arquivo> ][ -b <arquivo> <arquivo> ] [ -t <arquivo> ]\n", prog_name);
    fprintf(stderr, "\nOpções:\n");
    fprintf(stderr, "  -c <arquivo>             Modo de compressão,     Arquivo para compactar.\n");
    fprintf(stderr, "  -d <arquivo>             Modo de descompressão,  Arquivo para descompactar.\n");
    fprintf(stderr, "  -b <arquivo> <arquivo>   Modo de benchmark,      Arquivos para compactar e novo nome de arquivo descompactado.\n");
    fprintf(stderr, "  -t <arquivo>             Modo de teste,          Arquivo de de teste.\n");
    fprintf(stderr, "  -m                       Habilita o monitor.\n");
    fprintf(stderr, "  -h                       Mostra esta ajuda.\n");
}

void args_exit_if_mode_already_set(args_t *args, const char *prog_name){
    if(args->mode != ' '){
        fprintf(stderr, "Modo já escolhido (provavelmente foram selecionadas mais de uma flag e modo)\n");
        args_print_usage(prog_name);
        exit(1);
    }
}

void args_exit_if_file_dont_exists(char *filename){
    if(access(filename, F_OK) != 0){
        fprintf(stderr, "Arquivo: %s não está acessível\n", filename);
        exit(1);
    }
}

void args_parse_params(int argc,char *argv[], args_t *args){
    int opt;
    char *prog_name = argv[0];
    args->mode = ' ';
    args->show_monitor = 0;
    args->to_compress_filename = NULL;
    args->to_decompress_filename = NULL;

    if(argc == 1){
        args_print_usage(prog_name);
        exit(1);
    }

    while((opt = getopt(argc,argv,"hc:d:b:t:m")) != -1){
        switch(opt){
            case 'h':
                args_print_usage(prog_name);
                exit(0);
                break;
            case 'c':
                args_exit_if_mode_already_set(args, prog_name);
                args->mode = 'c';
                args_exit_if_file_dont_exists(optarg);
                args->to_compress_filename = optarg;
                break;
                
            case 'd':
                args_exit_if_mode_already_set(args, prog_name);
                args->mode = 'd';
                args_exit_if_file_dont_exists(optarg);
                args->to_decompress_filename = optarg;
                break;
                
            case 't':
                args_exit_if_mode_already_set(args, prog_name);
                args->mode = 't';
                args_exit_if_file_dont_exists(optarg);
                args->to_compress_filename = optarg;
                break;
        
            case 'b':
                args_exit_if_mode_already_set(args, prog_name);
                args->mode = 'b';
                args_exit_if_file_dont_exists(optarg);
                args->to_compress_filename = optarg;
                
                if (optind >= argc || argv[optind][0] == '-') {
                    fprintf(stderr, "Erro: A flag -b requer DOIS argumentos.\n");
                    args_print_usage(argv[0]);
                    exit(1);
                }
                args->to_decompress_filename = argv[optind];
                optind++; 
                break;
            
            case 'm':
                args->show_monitor = 1;
                break;
            case '?':
            default:
                args_print_usage(argv[0]);
                exit(1);
                break;
        }
    }

    if (optind < argc) {
        fprintf(stderr, "Erro: Argumentos inesperados.\n");
        args_print_usage(prog_name);
        exit(1);
    }

    printf("Mode: %c\n", args->mode);
    printf("Habilitar Monitor: %d\n", args->show_monitor);
    printf("Para comprimir: %s\n", args->to_compress_filename);
    printf("Para descomprimir: %s\n", args->to_decompress_filename);


}



#endif