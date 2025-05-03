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
#include <signal.h>
#include <sys/wait.h>

#include "utils/Cache.h"
#include "utils/Message.h"
#include "utils/Metadados.h"
#include "cache_holder.h"
#include "disk_writer.h"

int paralels_function (Message *msg, int (*func)(Message *msg));

int verifica_comando (Message *msg);

void error_message(Message *msg);

void sent_to_cache (Message *msg);

#endif