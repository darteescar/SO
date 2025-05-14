#!/bin/bash
# Script to executar ./dclient -l <key> butterfly
# Este script executa o comando -l de forma aleatória 1000 vezes com o número de documentos variando entre 0 e 1644.

# Mudar para a raiz do projeto (pasta acima de scripts)
cd "$(dirname "$0")/.."

NUM_TESTES=1000
MAX_DOCUMENTO=1644

for ((i=1; i<=NUM_TESTES; i++)); do
    key=$(( RANDOM % (MAX_DOCUMENTO + 1) ))
    echo "Executando ./dclient -l $key butterfly"
    echo Número do teste: $i 
    ./dclient -l "$key" butterfly
done

echo "Concluído: $NUM_TESTES execuções de ./dclient -l <key> butterfly"
