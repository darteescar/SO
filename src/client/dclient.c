#include "client/services.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_FIFO "server_fifo"

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Invalid entry\n");
        return -1;
    }

    Message *msg = init_message();

    int pid = getpid();

    create_message(msg, argv, argc, pid);

    char buffer[512];
    sprintf(buffer, "tmp/%d", pid);
    mkfifo(buffer, 0600);

    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return -1;
    }

    write(fd, msg, get_message_size(msg));
    close(fd);

    reply(msg);

    return 0;
}