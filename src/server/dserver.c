#include "server/functions.h"

#define SERVER_FIFO "server_fifo"

int main() {

    printf("[SERVER STARTED]\n");

    server();

    printf("[SERVER ENDED]\n");

    return 0;
}