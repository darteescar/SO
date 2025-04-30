#include "server/functions.h"

#define SERVER_FIFO "tmp/server_fifo"
#define CACHE_FIFO "tmp/cache_fifo"

int paralels_function (Message *msg, int (*func)(Message *msg)) {
    int fd[2];
    pid_t pid;
    int valor;

    // Cria o pipe
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(fd[0]);
        valor = func(msg);
        write(fd[1], &valor, sizeof(int));
        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);
        waitpid(pid, NULL, 0);
        read(fd[0], &valor, sizeof(int));
        close(fd[0]);
    }    
    return valor;
}

int verifica_comando (Message *msg) {
    printf("[SERVER] verificando comando\n");
    if (msg == NULL) {
        return 0;
    }
    char a = get_message_command(msg);

    switch (a) {
        case 'a':
        printf("[SERVER] verificando comando a\n"); 
            if (get_message_argc(msg) != 5) {
                return 0;
            }
            printf("[SERVER] a retornar 1\n");
            return 1;
        case 'c':
            if (get_message_argc(msg) != 2) {
                return 0;
            }
            return 1;
        case 'd':
            if (get_message_argc(msg) != 2) {
                return 0;
            }
            return 1;
        case 'l':
            if (get_message_argc(msg) != 3) {
                return 0;
            }
            return 1;
        case 's':
            if (get_message_argc(msg) == 2 || get_message_argc(msg) == 3) {
                return 1;
            }
            return 0;
        case 'f':
            if (get_message_argc(msg) != 1) {
                return 0;
            }
            return 1;
        case 'b':
            if (get_message_argc(msg) != 1) {
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

void sent_to_cache (Message *msg) {
    MetaMessage *metaMessage = createMetaMessage(msg);

    int fd = open(CACHE_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("Open send_to_cache_holder");
        return;
    }
    printf("Enviando para cache_fifo send_to_cache_holder\n");
    print_MT(metaMessage);
    char *str = to_disk_format_MT(metaMessage);
    write(fd, str, sizeof(str));
    printf("Enviou para cache_fifo send_to_cache_holder\n");
    close(fd);
}