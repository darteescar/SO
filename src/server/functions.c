#include "server/functions.h"
#define SERVER_FIFO "server_fifo"

Cache *exec_comando (Message *msg, Cache *docs, int *server_down, char *folder) {
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

Cache *Server_opcao_A(Message *msg, Cache *docs){
    
    int *pos_onde_foi_add = malloc(sizeof(int));
    if (pos_onde_foi_add == NULL) {
        perror("malloc");
        return NULL;
    }

    docs = add_documento(docs, msg, pos_onde_foi_add);
    char respostaA[51];

    sprintf(respostaA, "Documento %d adicionado\n", *pos_onde_foi_add);
    envia_resposta_cliente(respostaA, msg);

    free(pos_onde_foi_add);
    return docs;
}

void Server_opcao_C(Message *msg, Cache *docs){

    int keyC = get_key_msg(msg);
    int doc_existe = documento_existe(docs, keyC);
    char respostaC[512];
    if (doc_existe == 1) {
        if (get_docs_estado(docs, keyC) == EM_DISCO) {
            disco_to_cache(docs, keyC);
        }
        MetaDados* reply = get_documento(docs,keyC);
        char *str = MD_toString(reply, keyC);
        sprintf(respostaC, "%s\n", str);
        free(str);        
    } else if (doc_existe == -2) {
        sprintf(respostaC, "Não existe nenhum documento com a chave %d\n", keyC);
    
    } else if (doc_existe == -1) {
        sprintf(respostaC, "Posição Inválida\n");
    }

    envia_resposta_cliente(respostaC, msg);
}

Cache *Server_opcao_D(Message *msg, Cache *docs){

    int keyD = get_key_msg(msg);
    int flagD = remove_documento(docs, keyD);

    char respostaD[50];
    if (flagD == 1) {
        sprintf(respostaD, "O documento com a chave %d foi apagado\n", keyD);
    } else if (flagD == -1) {
        sprintf(respostaD, "Posição Inválida\n");
    } else if (flagD == -2){
        sprintf(respostaD, "Não existe nenhum documento com a chave %d\n", keyD);
    }

    envia_resposta_cliente(respostaD, msg);
    return docs;
}

// Esta função procura a keyword no documento ( se o documento nao estiver em cache, ele escreve-o na cache )
void Server_opcao_L(Message *msg, Cache *docs, char* folder) {
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

        envia_resposta_cliente(resposta, msg);
        return;
    } else if (flag == -1) {
        char resposta[200] = {0};
        sprintf(resposta, "Posição Inválida\n");

        envia_resposta_cliente(resposta, msg);
        return;
    }

    if(get_docs_estado(docs, key) == EM_DISCO) {//Meter o MetaDados de indice key em cache
        disco_to_cache(docs, key);
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

        envia_resposta_cliente(buffer, msg);
    }
}

void Server_opcao_S(Message *msg, Cache *docs, char* folder) {
    char *keyword = get_keyword_msg_s(msg);
    if (keyword == NULL) {
        perror("get_keyword_msg_s Server_opcao_S");
        return;
    }

    char *resposta = malloc(sizeof(char) * 100);
    if (resposta == NULL) {
        perror("Malloc Server_opcao_S");
        return;
    }
    int size = 0;
    int max_size = 100;

    int n_total = get_nTotal(docs);

    for (int i = 0; i < n_total; i++) {
        if (documento_existe(docs, i) == 1) {
            MetaDados *doc = get_anywhere_documento(docs, i);
            char filepath[100];
            sprintf(filepath, "%s%s", folder, get_MD_path(doc));
            printf("Filepath: %s\n", filepath);
            int pid = fork();
            if (pid == -1) {
                perror("Fork Server_opcao_S");
                free(resposta);
                return;
            }

            if (pid == 0) {
                execlp("grep", "grep", "-q", keyword, filepath, NULL);
                _exit(1);
            } else {
                int status;
                waitpid(pid, &status, 0);

                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    printf("i: %d, resposta: %s\n", i, resposta);
                    char num[20];
                    snprintf(num, sizeof(num), "%d", i);
                    int num_len = strlen(num);

                    // Expandir resposta se necessário
                    if (size + num_len + 3 > max_size) {
                        max_size *= 2;
                        resposta = realloc(resposta, sizeof(char) * max_size);
                        if (resposta == NULL) {
                            perror("Realloc Server_opcao_S");
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

    
    strcat(resposta, "\n");
    envia_resposta_cliente(resposta, msg);
}

void Server_opcao_F(Message *msg, Cache *docs){
    for(int i=0; i<get_max_docs(docs); i++){
        if (documento_existe(docs, (get_next_to_disc(docs)%get_max_docs(docs))) == 1) {
            escreve_em_disco(docs, get_next_to_disc(docs));
            inc_next_to_disc(docs);
        }  
    }
    char *resposta= "Servidor a terminar...\n";
    envia_resposta_cliente(resposta, msg);
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
    const char *resposta;
    char option = get_message_command(msg);  // Supondo que você tenha um método para pegar o comando da mensagem
    switch(option){
        case 'a':
            resposta = "[TRY] -a <title> <authors> <year> <path>\n";
            break;
        case 'c':
            resposta = "[TRY] -c <key>\n";
            break;
        case 'd':
            resposta = "[TRY] -d <key>\n";
            break;
        case 'l':
            resposta = "[TRY] -l <key> <keyword>\n";
            break;
        case 's':
            resposta = "[TRY] -s <key> <keyword>\n";
            break;
        case 'f':
            resposta = "[TRY] -f\n";
            break;
        default:
            resposta = "[TRY] <command>\n";
            break;
    }
    envia_resposta_cliente(resposta, msg);
}

void envia_resposta_cliente(const char *msg, Message *msg_cliente) {
    char fifo[50];
    sprintf(fifo, "tmp/%d", get_message_pid(msg_cliente));
    int fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    write(fd, msg, strlen(msg));
    close(fd);
}