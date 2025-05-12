#include "server/options.h"

Cache *Server_opcao_A(MetaDados *msg, Cache *docs) {

     int *pos_onde_foi_add = malloc(sizeof(int));
     if (pos_onde_foi_add == NULL) {
          perror("malloc");
          return NULL;
     }

     docs = add_documento(docs, msg, pos_onde_foi_add);

     char respostaA[51];
     sprintf(respostaA, "Documento %d adicionado\n", *pos_onde_foi_add);
     
     if ( get_MD_pid(msg) != -1 ) {
          envia_resposta_cliente(respostaA, msg);
     }

     free(pos_onde_foi_add);
     return docs;
}

void Server_opcao_C(MetaDados *msg, Cache *docs) {
     char *pedido = get_MD_something(msg, 1);
     int keyC = atoi(pedido);
     int doc_existe = documento_existe(docs, keyC);
     char respostaC[520];
     if (doc_existe == 1) {
          MetaDados *doc = get_anywhere_documento(docs, keyC);

          if (get_cache_estado(docs, keyC) == EM_DISCO && get_cache_flag(docs)==CACHE_ESTATICA) {// se tiver em cache não é preciso
               set_MD_buffer(doc, "-a");                                                       // se for cache com arreio dinamico tb não é preciso
               set_MD_flag(doc, DISK_INFO_CREATED);
               set_MD_argc(doc, 6);
               set_MD_pid(doc, -1);
               send_to_server(doc);
          }

          char *str = MD_toString(doc, keyC);
          sprintf(respostaC, "%s", str);
          free(str);
     } else if (doc_existe == -2) {
          sprintf(respostaC, "Não existe nenhum documento com a chave %d\n", keyC);

     } else if (doc_existe == -1) {
          sprintf(respostaC, "Posição Inválida\n");
     }

     envia_resposta_cliente(respostaC, msg);
}

Cache *Server_opcao_D(MetaDados *msg, Cache *docs) {
     char *pedido = get_MD_something(msg, 1);
     int keyD = atoi(pedido);
     int doc_existe = documento_existe(docs, keyD);

     char respostaD[50];
     if (doc_existe == 1) {
          docs = remove_file(docs, keyD);
          sprintf(respostaD, "Documento %d removido\n", keyD);
     } else if (doc_existe == -2) {
          sprintf(respostaD, "Não existe nenhum documento com a chave %d\n", keyD);
     } else if (doc_existe == -1) {
          sprintf(respostaD, "Posição Inválida\n");
     }

     envia_resposta_cliente(respostaD, msg);

     return docs;
}

void Server_opcao_L(MetaDados *msg, Cache *docs, char* folder) {
     char *key_b = get_MD_something(msg, 1);
     int key = atoi(key_b);
     int flag = documento_existe(docs, key);
     char *keyword = get_MD_something(msg, 2);
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

     MetaDados *doc = get_anywhere_documento(docs, key);
     char *path = get_MD_path(doc);

     char filepath[100];
     sprintf(filepath, "%s%s", folder, path);

     free_MD(doc);
     free(path);

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
          char buffer[15] = {0};

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

void Server_opcao_S(MetaDados *msg, Cache *docs, char* folder) {
     char *keyword = get_MD_something(msg, 1);
     if (keyword == NULL) {
          perror("get_keyword_msg_s Server_opcao_S");
          return;
     }
     char *n_filhos_buffer = get_MD_something(msg, 2);
     int n_filhos = 0;

     if (n_filhos_buffer == NULL) {
          n_filhos = 1; //caso não tenha sido especificado
     } else {
          n_filhos = atoi(n_filhos_buffer);
     }

     int n_total = get_Max_docs(docs);
     int fd[n_filhos][2];
     pid_t pids[n_filhos];

     for (int i = 0; i < n_filhos; i++) {
          if (pipe(fd[i]) == -1) {
               perror("pipe");
               return;
          }

          pids[i] = fork();
          if (pids[i] == -1) {
               perror("fork");
               return;
          }

          if (pids[i] == 0) {

               close(fd[i][0]);

               int start = i * (n_total / n_filhos);
               int end = (i == n_filhos - 1) ? n_total : (i + 1) * (n_total / n_filhos);

               for (int j = start; j < end; j++) {
                    if (documento_existe(docs, j)) {
                         MetaDados *doc = NULL;
                         doc = get_anywhere_documento(docs, j);

                         char filepath[50];
                         sprintf(filepath, "%s%s", folder, get_MD_path(doc));

                         pid_t pid_grep = fork();
                         if (pid_grep == 0) {
                              execlp("grep", "grep", "-q", keyword, filepath, NULL);
                              _exit(1);
                         }
                         int status;
                         waitpid(pid_grep, &status, 0);
                         if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                              write(fd[i][1], &j, sizeof(int));
                         }

                         free_MD(doc);
                    }
               }

               close(fd[i][1]);
               _exit(0);
          } else {
               close(fd[i][1]);
          }
     }

     // Prepara a resposta
     char *resposta = malloc(128);
     int size = 0, max_size = 128;
     strcpy(resposta, "[");
     size = 1;
     for (int i = 0; i < n_filhos; i++) {
          int doc_idx;
          while (read(fd[i][0], &doc_idx, sizeof(int)) > 0) {
               char num[12];
               snprintf(num, sizeof(num), "%d", doc_idx);

               if (size > 1) {
                    strcat(resposta, ", ");
                    size += 2;
               }

               strcat(resposta, num);
               size += strlen(num);

               if (size + 12 > max_size) {
                    max_size *= 2;
                    resposta = realloc(resposta, max_size);
               }
          }
          close(fd[i][0]);
     }
     strcat(resposta, "]\n");
     for (int i = 0; i < n_filhos; i++) {
          waitpid(pids[i], NULL, 0);
     }
     envia_resposta_cliente(resposta, msg);
     free(resposta);
}

Cache* Server_opcao_B(MetaDados *msg, Cache *docs) {
     Cache* novo = recupera_backup(docs,msg);
     char *resposta= "Backup recuperado.\n";
     envia_resposta_cliente(resposta, msg);

     return novo;
}

void Server_opcao_F(MetaDados *msg, Cache *docs) {

     all_Cache_to_Disc(docs);

     MetaDados *mt = init_MD();
     set_MD_disk_position(mt, -2);
     add_to_Disk(docs, mt);
     free_MD(mt);

     char *resposta= "Servidor a terminar...\n";
     envia_resposta_cliente(resposta, msg);
}

void envia_resposta_cliente(const char *msg, MetaDados *msg_cliente) {
     char fifo[24];
     sprintf(fifo, "tmp/%d", get_MD_pid(msg_cliente));
     int fd = open(fifo, O_WRONLY);
     if (fd == -1) {
          perror("Open envia_resposta_cliente");
          return;
     }
     write(fd, msg, strlen(msg));
     close(fd);
}