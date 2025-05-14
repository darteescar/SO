#include "utils/average_time_clients.h"

int average_time_clients() {
    int fd = open("tmp/times.txt", O_RDONLY);
    if (fd == -1) {
        perror("Erro ao abrir times.txt");
        return -1;
    }

    char buffer[BUFFER_SIZE];
    char linha[LINE_MAX];
    ssize_t bytes_read;
    int linha_pos = 0;
    double soma = 0.0;
    int contador = 0;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                linha[linha_pos] = '\0';
                double valor = atof(linha);
                soma += valor;
                contador++;
                linha_pos = 0; // reiniciar a linha
            } else {
                if (linha_pos < LINE_MAX - 1) {
                    linha[linha_pos++] = buffer[i];
                }
            }
        }
    }

    close(fd);

    if (bytes_read < 0) {
        perror("Erro ao ler o ficheiro");
        return -1;
    }

    if (contador == 0) {
        printf("Nenhum valor encontrado no ficheiro.\n");
        return -1;
    }

    double media = soma / contador;
    printf("Média dos tempos: %.6f segundos (%d registos)\n", media, contador);

    return 0;
}
