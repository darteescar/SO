#!/bin/bash
# Script to executar ./dclient -s butterfly <nºprocesso> de 1 a 10000

# Mudar para a raiz do projeto (pasta acima de scripts)
cd "$(dirname "$0")/.."

START=1
END=50

for ((i=START; i<=END; i++)); do
    echo "Executando ./dclient -s butterfly $i"
    echo Número do teste: $i 
    ./dclient -s butterfly "$i"
done

echo "Concluído: todas as pesquisas de $START até $END foram executadas."
