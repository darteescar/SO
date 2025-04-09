#include "server/functions.h"
#define SERVER_FIFO "server_fifo"

int exec_comando (Message *msg, Documentos **docs) {
    switch (get_message_command(msg)) {
        case 'a':
            // Criar o FIFO
            char fifo[50];
            sprintf(fifo, "tmp/%d", get_message_pid(msg));
            int *pos_onde_foi_add = malloc(sizeof(int));

            // Passar &docs para poder atualizar o ponteiro
            *docs = add_documento(*docs, msg, pos_onde_foi_add);

            int fd = open(fifo, O_WRONLY);
            write(fd, pos_onde_foi_add, sizeof(int));
            close(fd);
            break;
        case 'c':
            //char fifo2[50];
            //sprintf(fifo2, "tmp/%d", get_message_pid(msg));
            //int key = get_key_msg(msg);
            // Criar
            break;
        case 'd':
            // Criar o FIFO
            char fifo3[50];
            sprintf(fifo3, "tmp/%d", get_message_pid(msg));

            //Passa o bit ocupado a 0
            int key2 = get_key_msg(msg);
            int flag = remove_documento(*docs, key2);

            char resposta[100];
            if (flag == 1) {
                sprintf(resposta, "O documento com a chave %d foi apagado", key2);
            } else if (flag == -1) {
                sprintf(resposta, "Posição Inválida");
            } else if (flag == -2){
                sprintf(resposta, "Não existe nenhum documento com a chave %d", key2);
            }

            // Manda uma mensagem para o cliente a dizer que foi apagado ou não
            int fd3 = open(fifo3, O_WRONLY);
            write(fd3, resposta, sizeof(char)*100);
            close(fd3);
            break;
            // Apagar
            break;
        case 'l':
            // Listar
            break;
        case 's':
            // Pesquisa
            break;
        case 'f':
            return 0;
            break;
        default:
            // Comando inválido
    }
    return 1;
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

