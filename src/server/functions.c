#include "server/functions.h"
#define SERVER_FIFO "server_fifo"

char *exec_comando (Message *msg) {

    switch (get_message_command(msg)) {
        case 'a':
            MetaDados *data = create_metaDados(msg);
            print_metaDados(data);

            //Guardar os metadados na estrutura de dados
            char buffer[512];
            sprintf(buffer, "tmp/%d", get_message_pid(msg));

            int fd = open(buffer, O_WRONLY);
            write(fd, get_MD_key(data), 512);
            close(fd);
 
             
            break;
        case 'c':
 
             // Criar
 
            break;
        case 'd':
 
             // Apagar
 
            break;
        case 'l':
         
             // Listar
 
            break;
        case 's':
 
             // Pesquisa
 
            break;
        case 'f':
             
             // Encerra o servidor
 
            break;
        default:
             // Comando inválido
    }
 
     
    return "INVALID COMMAND";
 
}
 
 int verifica_comando (Message *msg) {

    if (msg == NULL) {
        return 0;
    }
    char a = get_message_command(msg);

    switch (a) {
        case 'a':
            if (get_message_argc(msg) != 4) {
                return 0;
            }
            return 1;
        case 'c':
            if (get_message_argc(msg) != 1) {
                return 0;
            }
            return 1;
        case 'd':
            if (get_message_argc(msg) != 1) {
                return 0;
            }
            return 1;
        case 'l':
            if (get_message_argc(msg) != 2) {
                return 0;
            }
            return 1;
        case 's':
            if (get_message_argc(msg) == 1 || get_message_argc(msg) == 2) {
                return 1;
            }
            return 0;
        case 'f':
            if (get_message_argc(msg) != 0) {
                return 0;
            }
            return 1;
        default:
            // Comando inválido
            return 0;
    }
     
     
 }
 
void error_message(char option) {
    switch(option){
        case 'a':
            printf("[TRY] -a <title> <authors> <year> <path>\n");
            break;
        case 'c':
            printf("[TRY] -c <key>\n");
            break;
        case 'd':
            printf("[TRY] -d <key>\n");
            break;
        case 'l':
            printf("[TRY] -l <key> <keyword>\n");
            break;
        case 's':
            printf("[TRY] -s <keyword>\n");
            break;
        case 'f':
            printf("[TRY] -f\n");
            break;
        default:
            printf("INVALID ENTRY\n");
    }
}

