#include "client/services.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_FIFO "server_fifo"

int main(int argc, char* argv[]){

    client(argc,argv);

    return 0;
}