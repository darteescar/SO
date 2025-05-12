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
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "Stack.h"
#include "Metadados.h"

#define DISK_WRITER_FIFO "tmp/disk_writer_fifo"

#define EM_CACHE 'c'
#define EM_DISCO 'd'
#define LIVRE 'l'

#define CACHE_ESTATICA 0
#define CACHE_DINAMICA 1

typedef struct cache Cache;

/**
 * @brief Aloca memória para uma estrutura de dados do tipo Cache
 * 
 * @param max_docs Tamanho máximo da cache
 * @param flag Flag que indica se a cache é estática ou dinâmica (0 se for estática ou 1 se for dinâmica)
 * @return Cache* Apontador para a estrutura de dados alocada
 */
Cache *create_Cache(int max_docs, int flag);

/**
 * @brief Função que adiciona um MetaDados à cache
 * 
 * @param cache Apontador para a estrutura de dados Cache
 * @param data Apontador para a estrutura de dados MetaDados
 * @param pos_onde_foi_add Flag que indica a posição onde o MetaDados foi adicionado
 * @return Cache* Apontador para a estrutura de dados
 * @note Se a cache estiver cheia, o MetaDados será adicionado ao disco
 * @note Se a cache não estiver cheia, o MetaDados será adicionado à cache
 * @note Se a cache for dinâmica, o MetaDados será adicionado à cache
 */
Cache *add_documento(Cache *cache, MetaDados *data, int *pos_onde_foi_add);

/**
 * @brief Função que adiciona um MetaDados à cache de forma estática
 * 
 * @param docs Apontador para a estrutura de dados Cache
 * @param data Apontador para a estrutura de dados MetaDados
 * @param pos_onde_foi_add Flag que indica a posição onde o MetaDados foi adicionado
 * @return Cache* Apontador para a estrutura de dados
 */
Cache *add_documento_Estaticamente(Cache *docs, MetaDados *data, int *pos_onde_foi_add);

/**
 * @brief Função que adiciona um MetaDados à cache de forma dinâmica
 * 
 * @param docs Apontador para a estrutura de dados Cache
 * @param data Apontador para a estrutura de dados MetaDados
 * @param pos_onde_foi_add Flag que indica a posição onde o MetaDados foi adicionado
 * @return Cache* Apontador para a estrutura de dados
 */
Cache *add_documento_Dinamicamente(Cache *docs, MetaDados *data, int *pos_onde_foi_add);

/**
 * @brief Função que adiciona um MetaDados à cache numa dada posição
 * 
 * @param cache Apontador para a estrutura de dados Cache
 * @param data Apontador para a estrutura de dados MetaDados
 * @param pos Posição onde o MetaDados será adicionado
 */
void add_to_Cache(Cache *cache, MetaDados *data, int pos);

/**
 * @brief Função que adiciona um MetaDados ao ficheiro em disco
 * 
 * @param cache Apontador para a estrutura de dados Cache
 * @param data Apontador para a estrutura de dados MetaDados
 */
void add_to_Disk(Cache *cache, MetaDados *data);

/**
 * @brief Função que remove um MetaDados da cache
 * 
 * @param cache Apontador para a estrutura de dados Cache
 * @param pos Posição do MetaDados a ser removido
 * @return Cache* Apontador para a estrutura de dados Cache
 */
Cache *remove_file (Cache *cache, int pos);

/**
 * @brief Função que liberta a memória alocada para a estrutura de dados Cache
 * 
 * @param cache Apontador para a estrutura de dados Cache
 */
void free_Cache(Cache *cache);

/**
 * @brief Função que redimensiona o array de ocupados da cache
 * 
 * @param docs Apontador para a estrutura de dados Cache
 */
void redimensiona_ocupados(Cache *docs);

/**
 * @brief Função que redimensiona os arrays de ocupados e stack da cache
 * 
 * @param docs Apontador para a estrutura de dados Cache
 */
void redimensionar_auxiliares(Cache *docs);

/**
 * @brief Função que verifica se a informação de um documento existe na cache
 * 
 * @param docs Apontador para a estrutura de dados Cache
 * @param pos Key do documento a ser verificado
 * @return int 1 se a informação do documento existe, -1 se a posição for inválida, -2 se a informação do documento não existir
 */
int documento_existe(Cache *docs, int pos);

/**
 * @brief Função que retorna o MetaDados na cache ou no disco
 * 
 * @param docs Apontador para a estrutura de dados Cache
 * @param pos Key do documento a retornar
 * @return MetaDados* Apontador para a estrutura de dados MetaDados
 */
MetaDados *get_anywhere_documento(Cache *docs, int pos);

/**
 * @brief Função que imprime os MetaDados na cache
 * 
 * @param docs Apontador para a estrutura de dados Cache
 */
void print_Cache (Cache *docs);

/**
 * @brief Função que retorna o size da cache
 * 
 * @param docs Apontador para a estrutura de dados Cache
 * @return int Size da cache
 */
int get_Max_docs (Cache *docs);

/**
 * @brief Função que escreve todos os MetaDados da cache para o disco
 * 
 * @param docs Apontador para a estrutura de dados Cache
 */
void all_Cache_to_Disc (Cache *docs);

/**
 * @brief Função que retorna a flag da cache
 * 
 * @param docs Apontador para a estrutura de dados Cache
 * @return int Flag da cache(0 se for estática, 1 se for dinâmica)
 */
int get_cache_flag(Cache *docs);

/**
 * @brief Função que retorna o estado de um MetaDado na cache
 * 
 * @param cache Apontador para a estrutura de dados Cache
 * @param pos Posição do MetaDado a ser verificado
 * @return char Estado do MetaDado na cache
 */
char get_cache_estado(Cache *cache, int pos);

/**
 * @brief Função que reseta a cache
 * 
 * @param cache Apontador para a estrutura de dados Cache
 * @return Cache* Apontador para a nova estrutura de dados Cache
 */
Cache* reset_Cache(Cache *cache);

/**
 * @brief Função que recupera um backup apartir de um ficheiro binário
 * 
 * @param docs Apontador para a estrutura de dados Cache
 * @param msg Apontador para a estrutura de dados MetaDados
 * @return Cache* Apontador para a estrutura de dados Cache
 */
Cache* recupera_backup(Cache *docs,MetaDados *msg);


#endif