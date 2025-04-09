#include "client/services.h"

#define SERVER_FIFO "server_fifo"

void reply(Message *msg){
    switch (get_message_command(msg)) {
        case 'a':
            int pid = getpid();
            char buffer[50];
            sprintf(buffer, "tmp/%d", pid);

            int fifo = open(buffer, O_RDONLY);
            if (fifo == -1) {
                perror("open");
                return;
            }

            int key;
            read(fifo, &key, sizeof(int));

            char key_str[12];
            sprintf(key_str, "%d", key);

            write(1, "Documento ", 10);
            write(1, key_str, strlen(key_str));
            write(1, " indexado\n", 10);
            
            close(fifo);     
            return;
        case 'c':
 
             // Criar
 
            return;
        case 'd':
 
             // Apagar
 
            return;
        case 'l':
         
             // Listar
 
            return;
        case 's':
 
             // Pesquisa
 
            return;
        case 'f':
             
             // Encerra o servidor
 
            return;
        default:
            return;
             // Comando inválido
    }
}
