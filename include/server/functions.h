#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "utils/Message.h"
#include "utils/Metadados.h"
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

Documentos *exec_comando (Message *msg, Documentos *docs, int *server_down, char* folder);

Documentos *Server_opcao_A(Message *msg, Documentos *docs);

void Server_opcao_C(Message *msg, Documentos *docs);

Documentos *Server_opcao_D(Message *msg, Documentos *docs);

void Server_opcao_L (Message *msg, Documentos *docs);

int verifica_comando (Message *msg);

void error_message(char option);

#endif