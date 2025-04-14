#include "client/services.h"
#include "utils/Message.h"

#define SERVER_FIFO "server_fifo"

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Invalid entry\n");
        return -1;
    }

    Message *msg = init_message();

    int pid = getpid();

    create_message(msg, argv, argc, pid);

    //print_message(msg);

    // Enviar a mensagem para o servidor
    send_message(msg);

    // Espera pela resposta do servidor (FIFO)

    // Ler a resposta do servidor
    reply(msg);

    return 0;
}