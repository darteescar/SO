#include "client/services.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Invalid entry\n");
        return 1;
    }

    Message *msg = init_message();
    create_message(msg, argv, argc);

    int fd = open("server_fifo", O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    write(fd, msg, get_message_size(msg));
  
    close(fd);
    return 0;
}