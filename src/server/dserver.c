#include "server/functions.h"

#define SERVER_FIFO "tmp/server_fifo"
#define CACHE_FIFO "tmp/cache_fifo"

/**
 * @brief Função principal do servidor.
 * 
 * @param argc Número de argumentos.
 * @param argv Argumentos passados para o programa.
 */
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
        // Processo filho trata da cache
        cache_holder(cache_size, flag, folder);
        _exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Processo filho trata do disco
        write_to_disk();
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
            int valor = verifica_comando(msg);
            if (valor == 1) {
                pid_t pid = fork();

                if (pid < 0) {
                    perror("fork no server");
                    free_message(msg);
                    return -1;
                }

                if (pid == 0) {
                    // FILHO
                    send_MSG_to_cache(msg);
                    _exit(0);
                }
            } else if (valor == 2){ //-f
                send_MSG_to_cache(msg);
                free_message(msg);
                break;
            } else {
                error_message(msg);
            }
        } 
        free_message(msg);
    }

    waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);

    unlink(SERVER_FIFO);
    return 0;
}