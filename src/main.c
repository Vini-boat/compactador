#include "args.h"

#include "modes/compression_mode.h"
#include "modes/decompression_mode.h"
#include "modes/benchmark_mode.h"
#include "modes/test_mode.h"

int main(int argc, char *argv[]) {

    args_t args;
    args_parse_params(argc,argv,&args);

    switch(args.mode){
        case 'c':
            run_compression_mode(&args);
            break;
        case 'd':
            run_decompression_mode(&args);
            break;
        case 'b':
            run_benchmark_mode(&args);
            break;
        case 't':
            run_test_mode(&args);
            break;
        default:
            break;
    }

    return 0;
}