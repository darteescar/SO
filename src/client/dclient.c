#include "client/services.h"
#include "utils/Message.h"

#define SERVER_FIFO "tmp/server_fifo"

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Invalid entry\n");
        return -1;
    }

    Message *msg = init_message();

    int pid = getpid();

    create_message(msg, argv, argc, pid);

    // Enviar a mensagem para o servidor
    int x = send_message(msg);

    if (x == -1) {
        perror("Send_message na main do dclient");
        free_message(msg);
        return -1;
    } else {
        free_message(msg);
    }

    // Ler a resposta do servidor
    reply(msg);

    return 0;
}