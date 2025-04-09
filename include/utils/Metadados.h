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

typedef struct documentos Documentos;

void print_metaDados(MetaDados data);

void free_metaDados(MetaDados *data);

char* get_MD_titulo(MetaDados *data);

char* get_MD_path(MetaDados *data);

int get_MD_ano(MetaDados *data);

int get_MD_n_autores(MetaDados *data);

void create_metaDados(Message *msg, Documentos *doc, int i);

Documentos *create_documentos(int max_docs);

//Documentos *realoca_documentos (Documentos *docs);

Documentos *add_documento(Documentos *docs, Message *data, int *pos_onde_foi_add);

int remove_documento(Documentos *docs, int pos);

void print_documentos (Documentos *docs);

#endif