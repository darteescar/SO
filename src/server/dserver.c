#include "server/functions.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define SERVER_FIFO "tmp/server_fifo"
#define CACHE_FIFO "tmp/cache_fifo"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        write(1, "[TRY]: ./dserver <folder> <cache_size>\n", 40);
        return -1;
    }

    if (mkfifo(SERVER_FIFO, 0666) == -1) {
        perror("MKFIFO server_fifo");
    }

    char* folder = argv[1];
    int flag;
    int cache_size;

    if (argv[2] == NULL) {
        flag = CACHE_DINAMICA;
        cache_size = 10;
    } else {
        flag = CACHE_ESTATICA;
        cache_size = atoi(argv[2]);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        write_to_disk();
        _exit(0);
    }

    int fd = open(SERVER_FIFO, O_RDWR);
    if (fd == -1) {
        perror("Open server_fifo");
        return -1;
    }

    int *server_down = malloc(sizeof(int));
    if (!server_down) {
        perror("malloc server_down");
        return -1;
    }
     *server_down = 0;

    Cache *cache = create_Cache(cache_size, flag);

    while (1) {

        MetaDados *mt = init_MD();

        ssize_t bytes = read(fd, mt, get_MD_size(mt));

        if (bytes > 0) {
            int is_a_valid_command = verifica_comando(mt);
            if (is_a_valid_command == 1) {
                cache = process_message(mt, cache, server_down, folder);
                if (*server_down == 1) {
                    break;
                }

            } else {
                error_message(mt);
            }

        } else {
            perror("read");
        }

        int status;
        pid_t pid;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            //printf("Processo filho terminado (PID: %d)\n", pid);
        }

        //print_Cache(cache);

    }

    waitpid(pid2, NULL, 0);

    average_time_clients();

    close(fd);
    free(server_down);
    unlink(SERVER_FIFO);
    free_Cache(cache);
    return 0;
}