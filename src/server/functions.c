#include "server/functions.h"

#define SERVER_FIFO "tmp/server_fifo"
#define CACHE_FIFO "tmp/cache_fifo"

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
        perror("Open envia_resposta_cliente");
        return;
    }
    write(fd, resposta, strlen(resposta));
    close(fd);
    
}

void send_to_SERVER_again(MetaDados *mt){

    if (get_MD_command(mt) == 'a') {
        add_MD_info_server(mt);
    }

    set_MD_1vez(mt, 'a');

    int fd = open(SERVER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("Open server_fifo");
        return;
    }


    write(fd, mt, get_MD_size(mt));
    close(fd);
    free_MD(mt);
}