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

#define SERVER_STORAGE "tmp/server_storage"

#define EM_CACHE 'c'
#define EM_DISCO 'd'
#define LIVRE 'l'

typedef struct metaDados MetaDados;

typedef struct documentos Documentos;

void send_to_Cache(char* buffer, Documentos *doc, int i);

void free_metaDados(MetaDados *data);

void print_metaDados(MetaDados *data);

char* get_MD_titulo(MetaDados *data);

char* get_MD_path(MetaDados *data);

int get_MD_ano(MetaDados *data);

int get_MD_n_autores(MetaDados *data);

char** get_MD_autores(MetaDados *data);

char *MD_toString(MetaDados* data, int key);

Documentos *create_documentos(int max_docs);

Documentos *add_documento(Documentos *docs, Message *data, int *pos_onde_foi_add);

void redimensionar_ocupados(Documentos *docs);

int remove_documento(Documentos *docs, int pos);

int documento_existe(Documentos *docs, int pos);

MetaDados *get_documento (Documentos *docs, int pos);

void print_documentos (Documentos *docs);

int get_num_docs(Documentos *docs);

int get_nTotal(Documentos *docs);

int get_next_to_disc(Documentos *docs);

int get_max_docs(Documentos *docs);

void inc_next_to_disc(Documentos *docs);

void escreve_em_disco(Documentos *docs, int pos);

char *serializa_metaDados(MetaDados *data);

void disco_to_cache(Documentos *docs, int pos);

char* transform(char *data);

char get_docs_estado(Documentos *docs, int pos);



#endif