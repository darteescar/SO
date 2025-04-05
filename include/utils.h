#ifndef UTILS_H
#define UTILS_H

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

typedef struct message Message;

Message *init_message();

void create_message(Message *msg, char *argv[], int argc);

size_t get_message_size(Message *msg);

void print_message(Message *msg);

char get_message_command(Message *msg);

int get_message_argc(Message *msg);

#endif