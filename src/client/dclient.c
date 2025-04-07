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
        return;
    }

    Message *msg = init_message();

    int pid = getpid();

    create_message(msg, argv, argc, pid);

    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    write(fd, msg, get_message_size(msg));
    close(fd);

    if(get_message_command(msg)=='a'){
        fd=open(SERVER_FIFO, O_RDONLY);

        char key[512];
        read(fd,key,512);

        printf("Key: %s\n", key);
  
        close(fd);
    }
    return 0;
}