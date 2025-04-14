#include "server/functions.h"
#define SERVER_FIFO "server_fifo"

Documentos *exec_comando (Message *msg, Documentos *docs, int *server_down, char *folder) {
    switch (get_message_command(msg)) {
        case 'a':
            // Adicionar
            return Server_opcao_A(msg, docs);

        case 'c':
            // Consultar
            Server_opcao_C(msg, docs);
            break;

        case 'd':
            // Apagar
            return Server_opcao_D(msg, docs);

        case 'l':
            // Listar
            Server_opcao_L(msg, docs, folder);
            break;
        case 's':
            // Pesquisa
            Server_opcao_S(msg, docs, folder);
            break;
        case 'f':
            Server_opcao_F(msg, docs);
            *server_down = 1;
            break;
        default:
            // Comando inválido
    }
    return docs;
}

Documentos *Server_opcao_A(Message *msg, Documentos *docs){
    char fifoA[50];
    sprintf(fifoA, "tmp/%d", get_message_pid(msg));
    int *pos_onde_foi_add = malloc(sizeof(int));

    docs = add_documento(docs, msg, pos_onde_foi_add);

    int fdA = open(fifoA, O_WRONLY);
    if (fdA == -1) {
        perror("open");
        free(pos_onde_foi_add);
        return docs;
    }

    char respostaA[100]={0};
    sprintf(respostaA, "Documento %d adicionado\n", *pos_onde_foi_add);
    write(fdA, respostaA, strlen(respostaA));
    close(fdA);

    free(pos_onde_foi_add);
    return docs;
}

void Server_opcao_C(Message *msg, Documentos *docs){
    // Criar o FIFO
    char fifoC[50];
    sprintf(fifoC, "tmp/%d", get_message_pid(msg));

    int keyC = get_key_msg(msg);
    int doc_existe = documento_existe(docs, keyC);
    char respostaC[512]={0};
    if (doc_existe == 1) {
        MetaDados* reply = get_documento(docs,keyC);
        char *str = MD_toString(reply, keyC);
        sprintf(respostaC, "%s\n", str);
        free(str);        
    } else if (doc_existe == -2) {
        sprintf(respostaC, "Não existe nenhum documento com a chave %d\n", keyC);
    
    } else if (doc_existe == -1) {
        sprintf(respostaC, "Posição Inválida\n");
    }

    int fdC = open(fifoC, O_WRONLY);
    write(fdC, respostaC, strlen(respostaC));
    close(fdC);
}

Documentos *Server_opcao_D(Message *msg, Documentos *docs){
    char fifoD[50];
    sprintf(fifoD, "tmp/%d", get_message_pid(msg));

    int keyD = get_key_msg(msg);
    int flagD = remove_documento(docs, keyD);

    char respostaD[100]={0};
    if (flagD == 1) {
        sprintf(respostaD, "O documento com a chave %d foi apagado\n", keyD);
    } else if (flagD == -1) {
        sprintf(respostaD, "Posição Inválida\n");
    } else if (flagD == -2){
        sprintf(respostaD, "Não existe nenhum documento com a chave %d\n", keyD);
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
        printf("Keyword is NULL\n");
        return;
    }

    if (flag == -2){
        char resposta[200] = {0};
        sprintf(resposta, "Não existe nenhum documento com a chave %d\n", key);

        int fdL = open(fifoL, O_WRONLY);
        if (fdL == -1) {
            perror("open");
            return;
        }
        write(fdL, resposta, strlen(resposta));
        close(fdL);
        return;
    } else if (flag == -1) {
        char resposta[200] = {0};
        sprintf(resposta, "Posição Inválida\n");

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
        close(pipefd[1]);

        execlp("grep", "grep", "-c", keyword, filepath, NULL);
        perror("execlp");
        _exit(1);
    } else {
        // Processo pai
        close(pipefd[1]);
        char buffer[100] = {0};

        // Espera que o filho termine
        wait(NULL);

        // Lê o resultado do pipe
        ssize_t n = read(pipefd[0], buffer, sizeof(buffer));
        if (n == -1) {
            perror("read");
            close(pipefd[0]);
            return;
        }
        close(pipefd[0]);
        buffer[n] = '\0'; // Adiciona o terminador nulo

        // Envia o resultado para o cliente
        int fd = open(fifoL, O_WRONLY);
        if (fd == -1) {
            perror("open fifo cliente");
            return;
        }
        
        write(fd, buffer, n);
        close(fd);
    }
}

void Server_opcao_S(Message *msg, Documentos *docs, char* folder) {
    char fifoS[50];
    sprintf(fifoS, "tmp/%d", get_message_pid(msg));

    char *keyword = get_keyword_msg_s(msg);
    if (keyword == NULL) {
        perror("get_keyword_msg_s");
        return;
    }

    char *resposta = malloc(sizeof(char) * 100);
    if (resposta == NULL) {
        perror("malloc");
        return;
    }
    int size = 0;
    int max_size = 100;

    int num_docs = get_num_docs(docs);
    for (int i = 0; i < num_docs; i++) {
        if (documento_existe(docs, i) == 1) {
            MetaDados *doc = get_documento(docs, i);

            char filepath[100];
            sprintf(filepath, "%s%s", folder, get_MD_path(doc));

            int pid = fork();
            if (pid == -1) {
                perror("fork");
                free(resposta);
                return;
            }

            if (pid == 0) {
                // Processo filho: executa grep -q
                execlp("grep", "grep", "-q", keyword, filepath, NULL);
                // Se falhar
                _exit(1);
            } else {
                // Processo pai: espera e analisa o resultado
                int status;
                waitpid(pid, &status, 0);

                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    // Encontrou a keyword neste documento

                    char num[20];
                    snprintf(num, sizeof(num), "%d", i);
                    int num_len = strlen(num);

                    // Expandir resposta se necessário
                    if (size + num_len + 3 > max_size) {
                        max_size *= 2;
                        resposta = realloc(resposta, sizeof(char) * max_size);
                        if (resposta == NULL) {
                            perror("realloc");
                            return;
                        }
                    }

                    // Adicionar separador ou abrir lista
                    if (size == 0) {
                        strcpy(resposta, "[");
                        size = 1;
                    } else {
                        strcat(resposta, ", ");
                        size += 2;
                    }

                    strcat(resposta, num);
                    size += num_len;
                }
            }
        }
    }

    // Fechar a lista ou devolver lista vazia
    if (size > 0) {
        if (size + 2 > max_size) {
            resposta = realloc(resposta, sizeof(char) * (max_size + 2));
            if (resposta == NULL) {
                perror("realloc");
                return;
            }
        }
        strcat(resposta, "]");
    } else {
        strcpy(resposta, "[]");
    }

    // Envia a resposta para o cliente
    int fd = open(fifoS, O_WRONLY);
    if (fd == -1) {
        perror("open fifoS");
        free(resposta);
        return;
    }
    strcat(resposta, "\n");
    write(fd, resposta, strlen(resposta));
    close(fd);
    free(resposta);
}

void Server_opcao_F(Message *msg, Documentos *docs){
    char fifo[50];
    sprintf(fifo, "tmp/%d", get_message_pid(msg));

    int fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    char resposta[100]={0}; 
    sprintf(resposta, "Servidor a terminar...\n");
    write(fd, resposta, sizeof(resposta));
    close(fd);

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
 
void error_message(Message *msg) {
    char fifo[50];
    sprintf(fifo, "tmp/%d", get_message_pid(msg));
    int fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    char option = get_message_command(msg);
    switch(option){
        case 'a':
            write(fd, "[TRY] -a <title> <authors> <year> <path>\n", 42);
            break;
        case 'c':
            write(fd, "[TRY] -c <key>\n", 16);
            break;
        case 'd':
            write(fd, "[TRY] -d <key>\n", 16);
            break;
        case 'l':
            write(fd, "[TRY] -l <key> <keyword>\n", 26);
            break;
        case 's':
            write(fd, "[TRY] -s <key> <keyword>\n", 26);
            break;
        case 'f':
            write(fd, "[TRY] -f\n", 10);
            break;
        default:
            write(fd, "[TRY] <command>\n", 17);
            break;
    }
    close(fd);
}