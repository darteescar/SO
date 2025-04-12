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
    int flagC = 0;
    MetaDados* reply = consulta_documento(docs,keyC, &flagC);

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

void Server_opcao_L(Message *msg, Documentos *docs, char *folder) {
    char fifo[50];
    sprintf(fifo, "tmp/%d", get_message_pid(msg));

    char ficheiro[50];
    char resposta[200];

    char *keyword = get_keyword_msg(msg);
    int key = get_key_msg(msg);

    int flag = 0;
    MetaDados *data = consulta_documento(docs, key, &flag);

    if (flag != 1) {
        sprintf(resposta, "Não existe nenhum documento com a chave %d", key);

        int fdL = open(fifo, O_WRONLY);
        if (fdL == -1) {
            perror("open");
            return;
        }
        write(fdL, resposta, sizeof(char) * 100);
        close(fdL);
        return;
    }

    char *path = get_MD_path(data);
    sprintf(ficheiro, "%s/%s", folder, path);
    int fd = open(ficheiro, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    int count = 0;
    size_t n;
    char buffer[256];
    char line[1024];
    int line_index = 0;

    // Processar o ficheiro linha por linha
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        for (size_t i = 0; i < n; i++) {
            if (buffer[i] == '\n' || line_index >= sizeof(line) - 1) {
                line[line_index] = '\0'; // Finalizar a linha
                if (strstr(line, keyword) != NULL) {
                    count++;
                }
                line_index = 0; // Reiniciar o índice da linha
            } else {
                line[line_index++] = buffer[i];
            }
        }
    }

    // Verificar a última linha (caso não termine com '\n')
    if (line_index > 0) {
        line[line_index] = '\0';
        if (strstr(line, keyword) != NULL) {
            count++;
        }
    }

    close(fd);

    if (count > 0) {
        sprintf(resposta, "A palavra %s aparece em %d linha(s) no ficheiro %s", keyword, count, path);
    } else {
        sprintf(resposta, "A palavra %s não aparece em nenhuma linha no ficheiro %s", keyword, path);
    }

    int fdL = open(fifo, O_WRONLY);
    if (fdL == -1) {
        perror("open");
        return;
    }
    write(fdL, resposta, sizeof(char) * 200);
    close(fdL);
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

