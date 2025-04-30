#include "server/functions.h"

#define SERVER_FIFO "tmp/server_fifo"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        write(1, "[TRY]: ./dclient <folder> <cache_size>\n", 40);
        return -1;
    }

    write(1, "[SERVER STARTED]\n\n", 19);

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
        // Processo filho trata da cache
        cache_holder(cache_size, flag, folder);
        _exit(0);
    }


    // Impede processos zombie
    signal(SIGCHLD, SIG_IGN);

    while (1) {
        Message *msg = init_message();

        int fd = open(SERVER_FIFO, O_RDONLY);
        if (fd == -1) {
            perror("Open server_fifo");
            return -1;
        }

        ssize_t bytes = read(fd, msg, get_message_size(msg));
        close(fd);

        if (bytes > 0) {
            int valor = paralels_function(msg,verifica_comando);
            if (valor == 1) {
                pid_t pid = fork();

                if (pid < 0) {
                    perror("fork no server");
                    free_message(msg);
                    return -1;
                }

                if (pid == 0) {
                    // FILHO
                    sent_to_cache(msg);
                    _exit(0);
                }
            } else {
                error_message(msg);
            }
        } else {
            free_message(msg);
        }
    }

    unlink(SERVER_FIFO);
    write(1, "[SERVER ENDED]\n", 16);
    return 0;
}