#include "client/services.h"

#define SERVER_FIFO "tmp/server_fifo"

int send_message (Message *msg){
    char buffer[512];
    sprintf(buffer, "tmp/%d", get_message_pid(msg));
    mkfifo(buffer, 0600);

    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    ssize_t x = write(fd, msg, get_message_size(msg));
    close(fd);
    return x;
}

void reply(Message *msg){
    int pid = getpid();
    char fifo_path[512];
    sprintf(fifo_path, "tmp/%d", pid);

    int fifo = open(fifo_path, O_RDONLY);
    if (fifo == -1) {
        perror("open");
        return;
    }

    size_t buffer_size = 512;
    size_t total_read = 0;
    char *resposta = malloc(buffer_size);
    if (!resposta) {
        perror("malloc");
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
                perror("realloc");
                free(resposta);
                close(fifo);
                return;
            }
            resposta = temp;
        }
    }

    if (bytes == -1) {
        perror("read");
    } else {
        write(1, resposta, total_read);
    }

    free(resposta);
    close(fifo);
}