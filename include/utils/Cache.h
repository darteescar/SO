/**
 * @file Cache.h
 * @brief Header file for Cache.c
 * @author Group 37 de SO
 */
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

typedef struct cache Cache;

Cache *create_Cache(int max_docs, int flag);

Cache *add_documento(Cache *cache, MetaDados *data, int *pos_onde_foi_add);

Cache *add_documento_Estaticamente(Cache *docs, MetaDados *data, int *pos_onde_foi_add);

Cache *add_documento_Dinamicamente(Cache *docs, MetaDados *data, int *pos_onde_foi_add);

void add_to_Cache(Cache *cache, MetaDados *data, int pos);

void add_to_Disk(Cache *cache, MetaDados *data);

void add_to_Disk(Cache *cache, MetaDados *data);

Cache *remove_file (Cache *cache, int pos);

char *consult_file (Cache *cache, int pos);

void free_Cache(Cache *docs);

void redimensiona_ocupados(Cache *docs);
 
void redimensionar_auxiliares(Cache *docs);

int documento_existe(Cache *docs, int pos);

char get_docs_estado(Cache *docs, int pos);

MetaDados *get_anywhere_documento(Cache *docs, int pos);

void print_Cache (Cache *docs);

int get_Max_docs (Cache *docs);

void all_Cache_to_Disc (Cache *docs);

MetaDados* desserializa_MetaDados(int pos);

int get_cache_flag(Cache *docs);

void recupera_backup(Cache *docs);

#endif