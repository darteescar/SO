#!/bin/bash
# Script to executar ./dclient -d <key> 
# Este script executa o comando -d de forma aleatória 1000 vezes com o número de documentos variando entre 0 e 1644.

# Mudar para a raiz do projeto (pasta acima de scripts)
cd "$(dirname "$0")/.."

NUM_TESTES=1000  # Número de execuções
MAX_DOCUMENTO=1644

for ((i=0; i<NUM_TESTES; i++)); do
    num_doc=$(( RANDOM % (MAX_DOCUMENTO + 1) ))
    echo "Executando ./dclient -d $num_doc"
    echo Número do teste: $i 
    ./dclient -d "$num_doc"
done

echo "Concluído: $NUM_TESTES execuções de ./dclient -d <key>"