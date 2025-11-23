#ifndef MONITOR_PROC_C
#define MONITOR_PROC_C

#include "structures/stats_shm.h"
#include "args.h"

#include <stdio.h>
#include <unistd.h>


void print_comp_snap(stats_snap_t *snap,int lines_to_go_back){
    printf("=== MONITOR DE COMPRESSÃO ===\n");
    printf("\033[2KStatus:                %s\n", snap->comp_data.finished ? "Finalizado" : "Rodando...");
    printf("\033[2KTokens Lidos:          %ld\n", snap->comp_data.read_tokens_total);
    printf("\033[2KTokens Comprimidos:    %ld\n", snap->comp_data.compressed_tokens_total);
    printf("\033[2KTokens Sem Dicionário: %ld\n", snap->comp_data.tokens_not_in_dict_total);
    printf("\033[2KTamanho Original:      %lld bytes\n", snap->comp_data.original_size_bytes);
    if(lines_to_go_back != 0) fflush(stdout);
    printf("\033[%dA",lines_to_go_back);
    
}
void print_decomp_snap(stats_snap_t *snap, int lines_to_go_back){
    printf("=== MONITOR DE DECOMPRESSÃO ===\n");
    printf("\033[2KStatus:                %s\n", snap->decomp_data.finished ? "Finalizado" : "Rodando...");
    printf("\033[2KBytes Lidos:           %ld\n", snap->decomp_data.read_bytes_total);
    printf("\033[2KBytes Desomprimidos:   %ld\n", snap->decomp_data.decompressed_bytes_total);
    printf("\033[2KBytes Sem Dicionário:  %ld\n", snap->decomp_data.bytes_not_in_dict_total);
    printf("\033[2KTamanho Final:         %lld bytes\n", snap->decomp_data.final_size_bytes);
    if(lines_to_go_back != 0) fflush(stdout);
    printf("\033[%dA",lines_to_go_back);
}
void print_both_snap(stats_snap_t *snap){
    print_comp_snap(snap,0);
    printf("\n");
    print_decomp_snap(snap,13);
}


void run_monitor_proc(args_t *args, stats_shm_t *stats){
    stats_snap_t snap;
    while(1){
        stats_shm_get_snap(stats,&snap);
        if(args->mode == 'c'){
            print_comp_snap(&snap,6);
            if(snap.comp_data.finished){
                break;
            }
        }
        if(args->mode == 'd'){
            print_decomp_snap(&snap,6);
            if(snap.decomp_data.finished){
                break;
            }
        }
        if(args->mode == 'b' || args->mode == 't'){
            print_both_snap(&snap);
            if(snap.comp_data.finished && snap.decomp_data.finished){
                break;
            }
        }
        sleep(1);
    }
}

#endif