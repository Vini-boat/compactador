#!/bin/bash

set -e  # para o script em qualquer erro

export FINAL="comp"
export COMPILER="gcc"
export SRC="./src"
export INCL="./include"
export FLAGS="-g -fsanitize=address -I${INCL} -Wall -Wextra -pthread" # -I define o diretório com headers e os -W os avisos que serão mostrados

echo "Iniciando build..."

SOURCES=$(find "${SRC}" -type f -name "*.c")

echo "Arquivos SRC encontrados (\$SOURCES):"
for i in $SOURCES; do echo -e "\t${i}";done

echo "Compilação:"
echo -e "\t${COMPILER} ${FLAGS} \$SOURCES -o ${FINAL}"

${COMPILER} ${FLAGS} ${SOURCES} -o ${FINAL}

echo "Build concluído em ./${FINAL}"
