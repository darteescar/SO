#ifndef MESSAGE_H
#define MESSAGE_H

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

void create_message(Message *msg, char *argv[], int argc, int pid);

size_t get_message_size(Message *msg);

void print_message(Message *msg);

char get_message_command(Message *msg);

int get_message_argc(Message *msg);

char* get_message_buffer(Message *msg);

int get_message_pid(Message *msg);

int get_key_msg(Message *msg);

char *get_keyword_msg(Message *msg);

#endif