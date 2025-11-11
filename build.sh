#!/bin/bash

set -e  # para o script em qualquer erro

export FINAL="comp"
export COMPILER="gcc"
export SRC="./src"
export INCL="./include"
export FLAGS="-I${INCL} -Wall -Wextra -pthread" # -I define o diretório com headers e os -W os avisos que serão mostrados

echo "Iniciando build..."

SOURCES=$(find "${SRC}" -type f -name "*.c")

${COMPILER} ${FLAGS} ${SOURCES} -o ${FINAL}

echo "Build concluído em ./${FINAL}"
