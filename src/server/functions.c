#include "server/functions.h"
#define SERVER_FIFO "server_fifo"

char *exec_comando (char *buffer) {

     switch (buffer[1]) {
         case 'a':
 
             // Adicionar
 
             
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



void server(){
    // Criar FIFO do servidor (se não existir)
    if (mkfifo(SERVER_FIFO, 0666) == -1) {
        perror("mkfifo");
    }

    Message *msg = init_message();

    while (1) {
        int fd = open(SERVER_FIFO, O_RDONLY);
        if (fd == -1) {
            perror("open server_fifo");
            return;
        }

        ssize_t bytes = read(fd, msg, get_message_size(msg));
        close(fd);

        if (bytes > 0) {
            //print_message(msg);
           
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
}
