#include "server/cache_holder.h"

#define CACHE_FIFO "tmp/cache_fifo"

void cache_holder(int cache_size, int flag, char *folder) {
     Cache *cache = create_Cache(cache_size, flag);
 
     if (mkfifo(CACHE_FIFO, 0666) == -1 && errno != EEXIST) {
         perror("MKFIFO cache_fifo na cache_holder"); 
         return;
     }
 
     int *server_down = malloc(sizeof(int));
     if (!server_down) {
         perror("malloc server_down");
         return;
     }
     *server_down = 0;
 
     int fd_rd = open(CACHE_FIFO, O_RDONLY);
     if (fd_rd == -1) {
         perror("Open cache_fifo (leitura)");
         return;
     }

     int i = 0;

     while (1) {
 
         MetaDados *msg = init_MD(); // Aloca estrutura
 
         ssize_t bytes_read = read(fd_rd, msg, get_MD_size(msg));
 
         if (bytes_read > 0) {
               printf("Cache Holder: %d\n", i);
               i++;
             cache = exec_comando(msg, cache, server_down, folder);
             if (*server_down == 1) {
                 break;
             }
         } else if (bytes_read == 0) {
             // writer fechou FIFO — reabrir bloqueando até novo writer
             close(fd_rd);
             fd_rd = open(CACHE_FIFO, O_RDONLY);
         }
 
     }

     free(server_down);
     close(fd_rd);
     unlink(CACHE_FIFO);
} 

Cache *exec_comando (MetaDados *msg, Cache *docs, int *server_down, char *folder) {
     switch (get_MT_command(msg)) {
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
          case 'b':
               // Backup
               Server_opcao_B(msg, docs);
               break;
          default:
     }
     return docs;
}

Cache *Server_opcao_A(MetaDados *msg, Cache *docs) {

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

void Server_opcao_C(MetaDados *msg, Cache *docs) {

     int keyC = get_MT_key(msg);
     int doc_existe = documento_existe(docs, keyC);
     char respostaC[520];
     if (doc_existe == 1) {
          char *str = consult_file(docs,keyC);
          sprintf(respostaC, "%s\n", str);
          free(str);        
     } else if (doc_existe == -2) {
          sprintf(respostaC, "Não existe nenhum documento com a chave %d\n", keyC);

     } else if (doc_existe == -1) {
          sprintf(respostaC, "Posição Inválida\n");
     }

     envia_resposta_cliente(respostaC, msg);
}

Cache *Server_opcao_D(MetaDados *msg, Cache *docs) {

     int keyD = get_MT_key(msg);
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
     int key = get_MT_key(msg);
     int flag = documento_existe(docs, key);
     char *keyword = get_MT_keyword(msg);
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

     // Obtem o path do documento
     char filepath[100];
          if (get_cache_flag(docs) == 0) {
               // Se a cache for estática
               sprintf(filepath, "%s%s", folder, get_MD_path(get_documento_cache(docs, key)));
          } else {
               // Se a cache for dinâmica
               sprintf(filepath, "%s%s", folder, get_MD_path(get_anywhere_documento(docs, key)));
          }

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

void Server_opcao_S(MetaDados *msg, Cache *docs, char* folder) {
     char *keyword = get_MT_keyword_s(msg);
     if (keyword == NULL) {
          perror("get_keyword_msg_s Server_opcao_S");
          return;
     }
     int n_filhos = get_MT_nProcessos_s(msg);
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

               for (int j = i; j < n_total; j += n_filhos) { // round-robin
                    if (documento_existe(docs, j)) {
                         MetaDados *doc = NULL;
                         if (get_cache_flag(docs) == 0){
                              doc = get_anywhere_documento(docs, j);
                         } else {
                              doc = get_documento_cache(docs, j);
                         }

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

void Server_opcao_B(MetaDados *msg, Cache *docs) {
     recupera_backup(docs);

     char *resposta= "Backup recuperado.\n";
     envia_resposta_cliente(resposta, msg);

}

void Server_opcao_F(MetaDados *msg, Cache *docs) {

     all_Cache_to_Disc(docs);

     char *resposta= "Servidor a terminar...\n";
     envia_resposta_cliente(resposta, msg);
}

void envia_resposta_cliente(const char *msg, MetaDados *msg_cliente) {
     char fifo[50];
     sprintf(fifo, "tmp/%d", get_MT_pid(msg_cliente));
     int fd = open(fifo, O_WRONLY);
     if (fd == -1) {
          perror("Open envia_resposta_cliente");
          return;
     }
     write(fd, msg, strlen(msg));
     close(fd);
 }
 