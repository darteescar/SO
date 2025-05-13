#!/bin/bash
# Script to executar ./dclient -l <key> butterfly

NUM_TESTES=100000
MAX_DOCUMENTO=1644

for ((i=1; i<=NUM_TESTES; i++)); do
    key=$(( RANDOM % (MAX_DOCUMENTO + 1) ))
    echo "Executando ./dclient -l $key butterfly"
    ./dclient -l "$i" butterfly
done

echo "Concluído: $NUM_TESTES execuções de ./dclient -l <key> butterfly"
