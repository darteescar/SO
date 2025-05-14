#!/bin/bash

NUM_TESTES=100000  # Número de execuções
MAX_DOCUMENTO=1644

for ((i=0; i<NUM_TESTES; i++)); do
    num_doc=$(( RANDOM % (MAX_DOCUMENTO + 1) ))
    echo "Executando ./dclient -c $num_doc"
    ./dclient -c "$num_doc"
done
