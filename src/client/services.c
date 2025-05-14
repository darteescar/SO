#include "client/services.h"

int send_message (MetaDados *msg){
    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("open na send_message");
        return -1;
    }

    ssize_t x = write(fd, msg, get_MD_size(msg));
    close(fd);
    return x;
}

void reply(){
    int pid = getpid();
    char fifo_path[24];
    sprintf(fifo_path, "tmp/%d", pid);

    int fifo = open(fifo_path, O_RDONLY);
    if (fifo == -1) {
        perror("open na reply");
        return;
    }

    size_t buffer_size = 520;
    size_t total_read = 0;
    char *resposta = malloc(buffer_size);
    if (!resposta) {
        perror("malloc na reply");
        close(fifo);
        return;
    }

    ssize_t bytes;
    while ((bytes = read(fifo, resposta + total_read, buffer_size - total_read)) > 0) {
        total_read += bytes;

        // Se buffer estiver cheio, duplicamos
        if (total_read == buffer_size) {
            buffer_size *= 2;
            char *temp = realloc(resposta, buffer_size);
            if (!temp) {
                perror("realloc na reply");
                free(resposta);
                close(fifo);
                return;
            }
            resposta = temp;
        }
    }

    if (bytes == -1) {
        perror("read na reply");
    } else {
        write(1, resposta, total_read);
    }

    free(resposta);
    close(fifo);
    unlink(fifo_path);
}

void registar_desempenho(double tempo) {
    int fd = open("tmp/times.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1) {
        perror("Erro ao abrir times.txt na registar_desempenho");
        return;
    }

    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.6f\n", tempo);
    write(fd, buffer, strlen(buffer));
    close(fd);

    //printf("\nTempo decorrido: %.6f segundos\n", elapsed);
}
