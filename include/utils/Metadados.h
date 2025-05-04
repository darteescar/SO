#ifndef METADADOS_H
#define METADADOS_H

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
#include "Message.h"

#define FIELD_SEP "\x1E"
#define AUTHOR_SEP "\x1F"

#define SERVER_STORAGE "server_storage"

typedef struct metaDados MetaDados;

MetaDados *init_MD();

MetaDados *create_MD_1 (int pid , char *buffer);

MetaDados *criar_metaDados(char *buffer);

char* get_MD_path(MetaDados *data);

int get_MD_pos_in_disk(MetaDados *data);

void set_MD_disk_position(MetaDados *data, int pos);

char *MD_toString(MetaDados* data, int key);

char *to_disk_format_MD(MetaDados *data);

void print_MD(MetaDados *data);

char* from_disk_format_MD(char *data);

int get_MD_size (MetaDados *data);

char get_MD_command(MetaDados *msg);

char *get_MD_buffer(MetaDados *msg);

int get_MD_pid(MetaDados *msg);

char *get_MD_keyword(MetaDados *msg);

void set_MD_pid(MetaDados *msg, int pid);

void set_MD_buffer(MetaDados *msg, char *buffer);

void free_MD(MetaDados *data);

char *get_MD_something (MetaDados *msg, int n);

#endif