#include "server/functions.h"

#define SERVER_FIFO "server_fifo"

int main() {

    printf("[SERVER STARTED]\n");

    // Criar FIFO do servidor (se não existir)
    if (mkfifo(SERVER_FIFO, 0666) == -1) {
        perror("mkfifo");
    }

    Message *msg = init_message();

    while (1) {
        int fd = open(SERVER_FIFO, O_RDONLY);
        if (fd == -1) {
            perror("open server_fifo");
            return -1;
        }

        ssize_t bytes = read(fd, msg, get_message_size(msg));
        close(fd);

        if (bytes > 0) {
            print_message(msg);
           
            if (verifica_comando(msg) == 1) {
               printf("Comando válido\n");
                
               
            } else {
                error_message(get_message_command(msg)); // isto terá de ser alterado para um char
            }
        }

        if(get_message_command(msg)=='a'){
            MetaDados *data = create_metaDados(msg);
            //print_metaDados(data);

            //Guardar os metadados na estrutura de dados

            fd = open(SERVER_FIFO, O_WRONLY);
            write(fd,get_MD_key(data),512);
            close(fd);

        }
    }

    printf("[SERVER ENDED]\n");

    return 0;
}