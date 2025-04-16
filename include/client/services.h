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

int send_message(Message *msg);

void reply(Message *msg);


#endif