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
        flag = 1;
        cache_size = 10;
    } else {
        flag = 0;
        cache_size = atoi(argv[2]);
    }
/*
    pid_t pid = fork();
    if (pid == 0) {
        cache_holder(cache_size, flag, folder);
        _exit(0);
    }
*/
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
            int valor = verifica_comando(mt);
            if (valor == 1) {
                if (get_MD_command(mt) == 's' || get_MD_command(mt) == 'l') {
                    pid_t child = fork();

                    if (child < 0) {
                        perror("fork no server");
                        free_MD(mt);
                        return -1;
                    }
                    if (child == 0) {
                        exec_comando(mt, cache, server_down, folder);
                        _exit(0);
                    }
                }

                cache = exec_comando(mt, cache, server_down, folder);
                if (*server_down == 1) {
                    break;
                }

            } else {
                error_message(mt);
            }
        } else {
            perror("read");
        }

        while (waitpid(-1, NULL, WNOHANG) > 0) {}

    }

    //waitpid(pid, NULL, 0);
    waitpid(pid2, NULL, 0);

    //average_time_clients();

    unlink(SERVER_FIFO);
    return 0;
}