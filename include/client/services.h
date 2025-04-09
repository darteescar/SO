#ifndef CLIENT_SERVICES
#define CLIENT_SERVICES

#include "utils/Message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void reply(Message *msg);

void Client_opcao_A(Message *msg);

void Client_opcao_C(Message *msg);

void Client_opcao_D(Message *msg);

#endif