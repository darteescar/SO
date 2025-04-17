#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "utils/Cache.h"
#include "utils/Message.h"
#include "utils/Metadados.h"

Cache *exec_comando (Message *msg, Cache *docs, int *server_down, char* folder);

int paralels_function (Message *msg, int (*func)(Message *msg));

Cache *Server_opcao_A(Message *msg, Cache *docs);

void Server_opcao_C(Message *msg, Cache *docs);

Cache *Server_opcao_D(Message *msg, Cache *docs);

void Server_opcao_L (Message *msg, Cache *docs, char *folder);

void Server_opcao_S(Message *msg, Cache *docs, char *folder);

void Server_opcao_F(Message *msg, Cache *docs);

int verifica_comando (Message *msg);

void error_message(Message *msg);

void envia_resposta_cliente(const char *msg, Message *msg_cliente);

#endif