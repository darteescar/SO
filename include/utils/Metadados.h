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

typedef struct metaDados MetaDados;

char *generate_key();

void print_metaDados(MetaDados *data);

void free_metaDados(MetaDados *data);

char* get_MD_titulo(MetaDados *data);

char* get_MD_path(MetaDados *data);

int get_MD_ano(MetaDados *data);

char* get_MD_key(MetaDados *data);

int get_MD_n_autores(MetaDados *data);

MetaDados *create_metaDados(Message *msg);

#endif