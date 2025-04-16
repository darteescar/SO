#ifndef DOCUMENTOS_H
#define DOCUMENTOS_H

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
#include "Stack.h"
#include "Metadados.h"

#define EM_CACHE 'c'
#define EM_DISCO 'd'
#define LIVRE 'l'
#define EM_DISCO_E_CACHE 'b'

typedef struct cache Cache;

Cache *create_Cache(int max_docs);

Cache *add_documento(Cache *docs, Message *data, int *pos_onde_foi_add);

void print_Cache (Cache *docs);

int get_num_docs(Cache *docs);

int get_nTotal(Cache *docs);

int get_next_to_disc(Cache *docs);

void inc_next_to_disc(Cache *docs);

int get_max_docs(Cache *docs);

void escreve_em_disco(Cache *docs, int pos);
 
void redimensiona_ocupados(Cache *docs);

void redimensionar_auxiliares(Cache *docs);

int remove_documento(Cache *docs, int pos);

int documento_existe(Cache *docs, int pos);

MetaDados *get_documento(Cache *docs, int pos);

char* desserializa_metaDados(char *data);

void disco_to_cache(Cache *docs, int pos);
 
char get_docs_estado(Cache *docs, int pos);

void send_to_Cache(char *buffer, Cache *doc, int i);

void escreve_em_disco_from_Stack(Cache *docs,Message* msg, int pos);

void print_ocupados(Cache *docs);

MetaDados *get_anywhere_documento(Cache *docs, int pos);

#endif