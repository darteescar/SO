#include "client/services.h"

#define SERVER_FIFO "server_fifo"

void send_message (Message *msg){
    char buffer[512];
    sprintf(buffer, "tmp/%d", get_message_pid(msg));
    mkfifo(buffer, 0600);

    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    write(fd, msg, get_message_size(msg));
    close(fd);
}

void reply(Message *msg){
    // Criar o FIFO
    int pid = getpid();
    char buffer[512];
    sprintf(buffer, "tmp/%d", pid);

    int fifo = open(buffer, O_RDONLY);
    if (fifo == -1) {
        perror("open");
        return;
    }

    // Ler a resposta do servidor
    char resposta[512];
    int bytes_read = read(fifo, resposta, 512*sizeof(char));
    if (bytes_read == -1) {
        perror("read");
        close(fifo);
        return;
    }
    printf("%s", resposta);
    //write(1, resposta, bytes_read);
    close(fifo);
    
}