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

MetaDados *criar_metaDados(char *buffer);

char* get_MD_path(MetaDados *data);

int get_MD_pos_in_disk(MetaDados *data);

void set_disk_position(MetaDados *data, int pos);

char *MD_toString(MetaDados* data, int key);

char *to_disk_format(MetaDados *data);

void free_metaDados(MetaDados *data);

void print_metaDados(MetaDados *data);

char* from_disk_format(char *data);

#endif