#include "client/services.h"
#include "utils.h"

#define SERVER_FIFO "server_fifo"

void client(int argc, char* argv[]){
    if(argc < 2){
        printf("Invalid entry\n");
        return;
    }

    Message *msg = init_message();
    create_message(msg, argv, argc);

    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    write(fd, msg, get_message_size(msg));
    close(fd);

    if(get_message_command(msg)=='a'){
        fd=open(SERVER_FIFO, O_RDONLY);

        int key;
        read(fd,&key,sizeof(int));

        printf("Key: %d\n", key);
  
        close(fd);
    }
}