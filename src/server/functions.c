#include "server/functions.h"
#define SERVER_FIFO "server_fifo"

Documentos *exec_comando (Message *msg, Documentos *docs, int *server_down, char *folder) {
    switch (get_message_command(msg)) {
        case 'a':
            return Server_opcao_A(msg, docs);

        case 'c':
            Server_opcao_C(msg, docs);
            break;

        case 'd':
            return Server_opcao_D(msg, docs);

        case 'l':
            // Listar
            Server_opcao_L(msg, docs, folder);
            break;
        case 's':
            // Pesquisa
            break;
        case 'f':
            // Fechar
            *server_down = 1;
            break;
        default:
            // Comando inválido
    }
    return docs;
}

Documentos *Server_opcao_A(Message *msg, Documentos *docs){
    // Criar o FIFO
    char fifoA[50];
    sprintf(fifoA, "tmp/%d", get_message_pid(msg));
    int *pos_onde_foi_add = malloc(sizeof(int));

    docs = add_documento(docs, msg, pos_onde_foi_add);

    int fdA = open(fifoA, O_WRONLY);
    write(fdA, pos_onde_foi_add, sizeof(int));
    close(fdA);
    //free(pos_onde_foi_add);
    return docs;
}

void Server_opcao_C(Message *msg, Documentos *docs){
    // Criar o FIFO
    char fifoC[50];
    sprintf(fifoC, "tmp/%d", get_message_pid(msg));

    int keyC = get_key_msg(msg);
    int doc_existe = documento_existe(docs, keyC);
    char respostaC[500];
    if (doc_existe == 1) {
        MetaDados* reply = get_documento(docs,keyC);
        char *str = MD_toString(reply, keyC);
        sprintf(respostaC, "%s", str);
        free(str);        
    } else if (doc_existe == -2) {
        sprintf(respostaC, "Não existe nenhum documento com a chave %d", keyC);
    
    } else if (doc_existe == -1) {
        sprintf(respostaC, "Posição Inválida");
    }

    int fdC = open(fifoC, O_WRONLY);
    write(fdC, respostaC, sizeof(char)*500);
    close(fdC);
}

Documentos *Server_opcao_D(Message *msg, Documentos *docs){
    char fifoD[50];
    sprintf(fifoD, "tmp/%d", get_message_pid(msg));

    int keyD = get_key_msg(msg);
    int flagD = remove_documento(docs, keyD);

    char respostaD[100];
    if (flagD == 1) {
        sprintf(respostaD, "O documento com a chave %d foi apagado", keyD);
    } else if (flagD == -1) {
        sprintf(respostaD, "Posição Inválida");
    } else if (flagD == -2){
        sprintf(respostaD, "Não existe nenhum documento com a chave %d", keyD);
    }

    int fdD = open(fifoD, O_WRONLY);
    write(fdD, respostaD, sizeof(char)*100);
    close(fdD);

    return docs;
}
 
void Server_opcao_L(Message *msg, Documentos *docs, char* folder) {
    char fifoL[50];
    sprintf(fifoL, "tmp/%d", get_message_pid(msg));

    int key = get_key_msg(msg);
    int flag = documento_existe(docs, key);
    char *keyword = get_keyword_msg(msg);
    if (keyword == NULL) {
        perror("get_keyword_msg");
        return;
    }

    char resposta[200];
    if (flag == -2){
        sprintf(resposta, "Não existe nenhum documento com a chave %d", key);

        int fdL = open(fifoL, O_WRONLY);
        if (fdL == -1) {
            perror("open");
            return;
        }
        write(fdL, resposta, sizeof(char) * 200);
        close(fdL);
        return;
    } else if (flag == -1) {
        sprintf(resposta, "Posição Inválida");

        int fdL = open(fifoL, O_WRONLY);
        if (fdL == -1) {
            perror("open");
            return;
        }
        write(fdL, resposta, sizeof(char) * 200);
        close(fdL);
        return;
    }

    // Obtem o path do documento
    char filepath[100];
    sprintf(filepath, "%s%s", folder, get_MD_path(get_documento(docs, key)));

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Processo filho
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        execlp("grep", "grep", "-c", keyword, filepath, NULL);
        close(pipefd[1]);

        _exit(1);
    } else {
        // Processo pai
        close(pipefd[1]);
        char buffer[100];
        read(pipefd[0], buffer, 100*sizeof(char));

        close(pipefd[0]);

        // Espera que o filho termine
        wait(NULL);

        // Envia o resultado para o cliente
        int fd = open(fifoL, O_WRONLY);
        if (fd == -1) {
            perror("open fifo cliente");
            return;
        }
        write(fd, buffer, 100*sizeof(char));
        close(fd);
    }
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
