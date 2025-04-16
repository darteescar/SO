#include "server/functions.h"

#define SERVER_FIFO "tmp/server_fifo"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        write(1, "[TRY]: ./dclient <folder> <cache_size>\n",40);
        return -1;
    }
    
    write(1, "[SERVER STARTED]\n\n", 19);

    // Criar FIFO do servidor (se não existir)
    if (mkfifo(SERVER_FIFO, 0666) == -1) {
        perror("mkfifo");
    }

    char* folder = argv[1];
    int cache_size = atoi(argv[2]); // definir que se o segundo argumento for nulo, o array é dinâmico

    Cache *docs = create_Cache(cache_size);
    int *server_down = malloc(sizeof(int));
    *server_down = 0;

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
            if (verifica_comando(msg) == 1) {
                docs = exec_comando(msg, docs,server_down,folder);
                //print_Cache(docs);
                if (*server_down == 1) {
                    //server_shutdown(docs);
                    break;
                }
            } else {
                error_message(msg);
            }
        }
    }

    print_Cache(docs);

    write(1, "[SERVER ENDED]\n", 16);

    return 0;
}
