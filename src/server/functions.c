#include "server/functions.h"

#define SERVER_FIFO "tmp/server_fifo"
#define CACHE_FIFO "tmp/cache_fifo"

int verifica_comando (Message *msg) {
    if (msg == NULL) {
        return 0;
    }
    char a = get_message_command(msg);
    int argc = get_message_argc(msg);
    switch (a) {
        case 'a':
            if (argc != 5) {
                return 0;
            }
            return 1;
        case 'c':
            if (argc != 2) {
                return 0;
            }
            return 1;
        case 'd':
            if (argc != 2) {
                return 0;
            }
            return 1;
        case 'l':
            if (argc != 3) {
                return 0;
            }
            return 1;
        case 's':
            if (argc == 2 || argc == 3) {
                return 1;
            }
            return 0;
        case 'f':
            if (argc != 1) {
                return 0;
            }
            return 2;
        case 'b':
            if (argc != 1) {
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
    char option = get_message_command(msg);
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
            resposta = "[TRY] -b\n";
            break;
        default:
            resposta = "[TRY] <command>\n";
            break;
    }

    char fifo[50];
    sprintf(fifo, "tmp/%d", get_message_pid(msg));
    int fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("Open envia_resposta_cliente");
        return;
    }
    write(fd, resposta, strlen(resposta));
    close(fd);
    
}

void send_MSG_to_cache (Message *msg) {

    MetaDados *MetaDados;

    char *buffer = get_message_buffer(msg);

    if (get_message_command(msg) == 'a') {
        char *buffer = get_message_buffer(msg);
        MetaDados = criar_metaDados(buffer);
        set_MD_buffer(MetaDados, buffer);
        set_MD_pid(MetaDados, get_message_pid(msg));
    } else {
        MetaDados = init_MD();
        set_MD_buffer(MetaDados, buffer);
        set_MD_pid(MetaDados, get_message_pid(msg));
    }
    free(buffer);

    int fd1 = open (CACHE_FIFO, O_WRONLY);
    if (fd1 == -1) {
        perror("Open cache_fifo");
        return;
    }

    write(fd1, MetaDados, get_MD_size(MetaDados));

    free_MD(MetaDados);
    close(fd1);       
}