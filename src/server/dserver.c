#include "server/functions.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define SERVER_FIFO "tmp/server_fifo"
#define CACHE_FIFO "tmp/cache_fifo"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        write(1, "[TRY]: ./dclient <folder> <cache_size>\n", 40);
        return -1;
    }

    if (mkfifo(SERVER_FIFO, 0666) == -1) {
        perror("MKFIFO server_fifo");
    }

    char* folder = argv[1];
    int flag;
    int cache_size;

    if (argv[2] == NULL) {
        flag = 1;
        cache_size = 10;
    } else {
        flag = 0;
        cache_size = atoi(argv[2]);
    }

    pid_t pid = fork();
    if (pid == 0) {
        cache_holder(cache_size, flag, folder);
        _exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        write_to_disk();
        _exit(0);
    }

    int fd = open(SERVER_FIFO, O_RDONLY);
    if (fd == -1) {
        perror("Open server_fifo");
        return -1;
    }

    while (1) {

        Message *msg = init_message();

        ssize_t bytes = read(fd, msg, get_message_size(msg));

        if (bytes > 0) {
            int valor = verifica_comando(msg);
            if (valor == 1) {
                pid_t child = fork();

                if (child < 0) {
                    perror("fork no server");
                    free_message(msg);
                    return -1;
                }

                if (child == 0) {
                    send_MSG_to_cache(msg);
                    _exit(0);
                }
            } else if (valor == 2){ //-f
                send_MSG_to_cache(msg);
                free_message(msg);
                printf("Comando -f recebido\n");
                break;
            } else {
                error_message(msg);
            }
        } else if (bytes == 0) {
            close(fd);
            fd = open(SERVER_FIFO, O_RDONLY);
        } else {
            perror("read");
        }

        while (waitpid(-1, NULL, WNOHANG) > 0) {}

        free_message(msg);
    }

    waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);

    average_time_clients();

    unlink(SERVER_FIFO);
    return 0;
}
