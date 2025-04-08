#ifndef DOCUMENTOS_H
#define DOCUMENTOS_H

#include "Metadados.h"
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

typedef struct documentos Documentos;

Documentos *create_documentos(int max_docs);

void add_documento(Documentos *docs, MetaDados *doc);

void increase_size_documentos(Documentos *docs);

MetaDados *get_documento(Documentos *docs, int index);

void print_documentos(Documentos *docs);



#endif

