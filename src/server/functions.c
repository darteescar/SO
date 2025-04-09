#include "server/functions.h"
#define SERVER_FIFO "server_fifo"

int exec_comando (Message *msg, Documentos **docs) {
    switch (get_message_command(msg)) {
        case 'a':
            Server_opcao_A(msg, docs);
            break;

        case 'c':
            Server_opcao_C(msg, docs);
            break;

        case 'd':
            Server_opcao_D(msg, docs);
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

void Server_opcao_A(Message *msg, Documentos **docs){
    // Criar o FIFO
    char fifoA[50];
    sprintf(fifoA, "tmp/%d", get_message_pid(msg));
    int *pos_onde_foi_add = malloc(sizeof(int));

    // Passar &docs para poder atualizar o ponteiro
    *docs = add_documento(*docs, msg, pos_onde_foi_add);

    int fdA = open(fifoA, O_WRONLY);
    write(fdA, pos_onde_foi_add, sizeof(int));
    close(fdA);
}

void Server_opcao_C(Message *msg, Documentos **docs){
    // Criar o FIFO
    char fifoC[50];
    sprintf(fifoC, "tmp/%d", get_message_pid(msg));

    int keyC = get_key_msg(msg);
    int flagC = 0;
    MetaDados* reply = consulta_documento(*docs,keyC, &flagC);

    char respostaC[500];
    if (flagC == 1) {
        char *str = MD_toString(reply, keyC);
        sprintf(respostaC, "%s", str);
        free(str);
    } else if (flagC == -1) {
        sprintf(respostaC, "Posição Inválida");
    } else if (flagC == -2){
        sprintf(respostaC, "Não existe nenhum documento com a chave %d", keyC); 
    }

    int fdC = open(fifoC, O_WRONLY);
    write(fdC, respostaC, sizeof(char)*500);
    close(fdC);
}

void Server_opcao_D(Message *msg, Documentos **docs){
    // Criar o FIFO
    char fifoD[50];
    sprintf(fifoD, "tmp/%d", get_message_pid(msg));

    //Passa o bit ocupado a 0
    int keyD = get_key_msg(msg);
    int flagD = remove_documento(*docs, keyD);

    char respostaD[100];
    if (flagD == 1) {
        sprintf(respostaD, "O documento com a chave %d foi apagado", keyD);
    } else if (flagD == -1) {
        sprintf(respostaD, "Posição Inválida");
    } else if (flagD == -2){
        sprintf(respostaD, "Não existe nenhum documento com a chave %d", keyD);
    }

    // Manda uma mensagem para o cliente a dizer que foi apagado ou não
    int fdD = open(fifoD, O_WRONLY);
    write(fdD, respostaD, sizeof(char)*100);
    close(fdD);
    
    // Apagar
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
 
void error_message(char option) {//MUDAR ISTO PARA MANDAR PARA O CLIENTE
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

