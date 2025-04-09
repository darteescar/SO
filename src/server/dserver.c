#include "server/functions.h"

#define SERVER_FIFO "server_fifo"

int main() {
    write(1, "[SERVER STARTED]\n\n", 19);

    // Criar FIFO do servidor (se não existir)
    if (mkfifo(SERVER_FIFO, 0666) == -1) {
        perror("mkfifo");
    }

    Documentos *docs = create_documentos(10);

    while (1) {
        Message *msg = init_message();
        int fd = open(SERVER_FIFO, O_RDONLY);
        if (fd == -1) {
            perror("open server_fifo");
            return -1;
        }

        ssize_t bytes = read(fd, msg, get_message_size(msg));
        close(fd);

        if (bytes > 0) {
            if (verifica_comando(msg) == 1) {
                if (exec_comando(msg, &docs) == 0) { break; }
            } else {
                error_message(get_message_command(msg));
            }
        }
    }

    print_documentos(docs);

    write(1, "[SERVER ENDED]\n", 16);

    return 0;
}
