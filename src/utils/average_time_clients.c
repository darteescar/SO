#include "utils/average_time_clients.h"

int average_time_clients() {
    FILE *f = fopen("times.txt", "r");
    if (f == NULL) {
        perror("Erro ao abrir times.txt");
        return -1;
    }

    double tempo, soma = 0.0;
    int contador = 0;

    while (fscanf(f, "%lf", &tempo) == 1) {
        soma += tempo;
        contador++;
    }

    fclose(f);

    if (contador == 0) {
        printf("Nenhum valor encontrado no ficheiro.\n");
        return -1;
    }

    double media = soma / contador;
    printf("Média dos tempos: %.6f segundos (%d registos)\n", media, contador);

    return 0;
}