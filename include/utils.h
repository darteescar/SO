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

typedef struct metaDados MetaDados;

char *generate_key(char *title, char *path, int year);

Message *init_message();

void create_message(Message *msg, char *argv[], int argc);

void print_metaDados(MetaDados *data);

void free_metaDados(MetaDados *data);

char* get_MD_titulo(MetaDados *data);

char* get_MD_path(MetaDados *data);

int get_MD_ano(MetaDados *data);

char* get_MD_key(MetaDados *data);

int get_MD_n_autores(MetaDados *data);

size_t get_message_size(Message *msg);

void print_message(Message *msg);

char get_message_command(Message *msg);

int get_message_argc(Message *msg);

char* get_message_buffer(Message *msg);

MetaDados *create_metaDados(Message *msg);


#endif