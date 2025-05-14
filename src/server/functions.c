#include "server/functions.h"

Cache *process_message(MetaDados *mt, Cache *cache, int *server_down, char *folder) {
    char comando = get_MD_command(mt);
    switch (comando) {
        case 'a':
            if ( get_MD_flag(mt) == DISK_INFO_NOT_CREATED ) {
                
                add_MD_info_server(mt);

                set_MD_flag(mt, DISK_INFO_CREATED);
                
            } 
            cache = Server_opcao_A(mt, cache);
            break;
        case 'c':
            pid_t child_5 = fork();
            if (child_5 < 0) {
                perror("fork no server");
                free_MD(mt);
                return NULL;
            }
            if (child_5 == 0) {
                Server_opcao_C(mt, cache);
                _exit(0);
            }
            break;
        case 'd':
            cache = Server_opcao_D(mt, cache);
            break;
        case 'l':
            pid_t child_2 = fork();

            if (child_2 < 0) {
                perror("fork no server");
                free_MD(mt);
                return NULL;
            }
            if (child_2 == 0) {
                Server_opcao_L(mt, cache, folder);
                _exit(0);
            }

            break;
        case 'f':
            Server_opcao_F(mt, cache);
            *server_down = 1;
            break;
        case 'b':
            cache = Server_opcao_B(mt, cache);
            break;
        case 's':
            pid_t child_3 = fork();

            if (child_3 < 0) {
                perror("fork no server");
                free_MD(mt);
                return NULL;
            }
            if (child_3 == 0) {
                Server_opcao_S(mt, cache, folder);
                _exit(0);
            }
            break;
        default:
            error_message(mt);
            free_MD(mt);
            return NULL;
    }

    return cache;
}

int verifica_comando (MetaDados *msg) {
    if (msg == NULL) {
        return 0;
    }
    char a = get_MD_command(msg);
    int argc = get_MD_argc(msg);
    switch (a) {
        case 'a':
            if (argc != 6) {
                return 0;
            }
            return 1;
        case 'c':
            if (argc != 3) {
                return 0;
            }
            return 1;
        case 'd':
            if (argc != 3) {
                return 0;
            }
            return 1;
        case 'l':
            if (argc != 4) {
                return 0;
            }
            return 1;
        case 's':
            if (argc == 3 || argc == 4) {
                return 1;
            }
            return 0;
        case 'f':
            if (argc != 2) {
                return 0;
            }
            return 1;
        case 'b':
            if (argc != 3) {
                return 0;
            }
            return 1;
        default:
            // Comando inválido
            return 0;
    }
}

void error_message(MetaDados *msg) {
    const char *resposta;
    char option = get_MD_command(msg);
    switch(option){
        case 'a':
            resposta = "[TRY] -a \"title\" \"authors\" \"year\" \"path\" \n";
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
        case 'b':
            resposta = "[TRY] -b <path>\n";
            break;
        default:
            resposta = "[TRY] <command>\n";
            break;
    }

    char fifo[50];
    sprintf(fifo, "tmp/%d", get_MD_pid(msg));
    int fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("Open error_message");
        return;
    }
    write(fd, resposta, strlen(resposta));
    close(fd);
    
}

void send_to_server(MetaDados *mt) {
    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("Open server_fifo");
        return;
    }

    write(fd, mt, get_MD_size(mt));
    close(fd);
}