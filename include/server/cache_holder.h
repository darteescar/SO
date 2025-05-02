#ifndef CACHE_HOLDER_H
#define CACHE_HOLDER_H

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

void cache_holder(int cache_size, int flag, char *folder);

Cache *exec_comando (MetaDados *msg, Cache *docs, int *server_down, char *folder);

Cache *Server_opcao_A(MetaDados *msg, Cache *docs);

void Server_opcao_C(MetaDados *msg, Cache *docs);

Cache *Server_opcao_D(MetaDados *msg, Cache *docs);

void Server_opcao_L(MetaDados *msg, Cache *docs, char* folder);

void Server_opcao_S(MetaDados *msg, Cache *docs, char* folder);

void Server_opcao_B(MetaDados *msg, Cache *docs);

void Server_opcao_F(MetaDados *msg, Cache *docs);

void envia_resposta_cliente(const char *msg, MetaDados *msg_cliente);

#endif