#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "utils/Message.h"
#include "utils/Metadados.h"
#include "utils/Documentos.h"
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

int exec_comando (Message *msg, Documentos *docs);

int verifica_comando (Message *msg);

void error_message(char option);

#endif