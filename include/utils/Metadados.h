/**
 * @file Metadados.h
 * @brief Header file for Metadados.c
 * @author Group 37 de SO
 */
#ifndef METADADOS_H
#define METADADOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIELD_SEP "\x1E"
#define AUTHOR_SEP "\x1F"

#define SERVER_STORAGE "server_storage"

#define MAX_TITULO 512
#define MAX_AUTORES 512
#define MAX_PATH 10

#define DISK_INFO_CREATED 'a'
#define DISK_INFO_NOT_CREATED 'b'

typedef struct metaDados MetaDados;

/**
 * @brief Aloca memória para uma estrutura de dados do tipo MeatDados
 * 
 * @return MetaDados* Apontador para a estrutura de dados alocada
 */
MetaDados *init_MD();

/**
 * @brief Função que adiciona informações do cliente à estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @param argv Argumentos do cliente
 * @param argc Número de argumentos do cliente
 * @param pid PID do cliente
 */
void add_MD_info_client (MetaDados *msg, char **argv, int argc, int pid);

/**
 * @brief Função que preenche campos relacionados à meta informação usando os dados preenchidos pelo cliente
 * 
 * @param msg Apontador para a estrutura de dados
 */
void add_MD_info_server (MetaDados *msg);

/**
 * @brief Função que cria uma estrutura de dados do tipo MetaDados a partir de um buffer
 * 
 * @param buffer Buffer a ser usado para criar a estrutura de dados
 * @return MetaDados* Apontador para a estrutura de dados criada
 */
MetaDados *criar_metaDados(char *buffer);

/**
 * @brief Função que desserializa um MetaDados a partir do disco
 * 
 * @param pos Posição do MetaDados a ser desserializado no disco
 * @return MetaDados* Apontador para os MetaDados lidos do disco
 */
MetaDados* desserializa_MetaDados(int pos);

/**
 * @brief Função que retorna o caminho do arquivo armazenado na estrutura de dados
 * 
 * @param data Apontador para a estrutura de dados
 * @return char* Caminho do arquivo armazenado na estrutura
 */
char* get_MD_path(MetaDados *data);

/**
 * @brief Função que retorna a key do MetaDados armazenado na estrutura de dados
 * 
 * @param data Apontador para a estrutura de dados
 * @return int Key do MetaDados armazenado na estrutura
 */
int get_MD_pos_in_disk(MetaDados *data);

/**
 * @brief Função que retorna o buffer armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @return char* Buffer armazenado na estrutura
 */
char *get_MD_buffer(MetaDados *msg);

/**
 * @brief Função que retorna o argc armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @return int argc armazenado na estrutura
 */
int get_MD_argc(MetaDados *msg);

/**
 * @brief Função que retorna o PID armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @return int PID armazenado na estrutura
 */
int get_MD_pid(MetaDados *msg);

/**
 * @brief Função que retorna o campo flag armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @return char flag armazenado na estrutura
 */
char get_MD_flag(MetaDados *mt);

/**
 * @brief Função que retorna o tamanho da estrutura de dados MetaDados
 * 
 * @param data Apontador para a estrutura de dados
 * @return int Tamanho da estrutura de dados
 */
int get_MD_size (MetaDados *data);

/**
 * @brief Função que retorna o comando present no buffer armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @return char Comando armazenado no buffer
 */
char get_MD_command(MetaDados *msg);

/** 
 * @brief Função que retorna o valor de um campo específico armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @param n Número do campo a ser retornado
 * @return char* Valor do campo armazenado na estrutura
 */
char *get_MD_something (MetaDados *msg, int n);

/**
 * @brief Função que define a posição do MetaDados armazenado na estrutura de dados
 * 
 * @param data Apontador para a estrutura de dados
 * @param pos Posição a ser definida
 */
void set_MD_disk_position(MetaDados *data, int pos);

/**
 * @brief Função que define o buffer armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @param buffer Buffer a ser definido
 */
void set_MD_buffer(MetaDados *msg, char *buffer);

/**
 * @brief Função que define o argc armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @param argc argc a ser definido
 */
void set_MD_argc(MetaDados *msg, int argc);

/**
 * @brief Função que define o PID armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @param pid PID a ser definido
 */
void set_MD_pid(MetaDados *msg, int pid);

/**
 * @brief Função que define o campo flag armazenado na estrutura de dados
 * 
 * @param msg Apontador para a estrutura de dados
 * @param valor Valor a ser definido
 */
void set_MD_flag(MetaDados *mt, int valor);

/**
 * @brief Função que retorna uma string com os dados do MetaDados correspondente a uma dada key
 * 
 * @param data Apontador para a estrutura de dados
 * @param key Key do MetaDados armazenado (ou na Cache ou no disco)
 * @return char* String com os dados do MetaDados
 */
char *MD_toString(MetaDados* data, int key);

/**
 * @brief Função que converte os dados do MetaDados para uma string no formato de disco
 * 
 * @param data Apontador para a estrutura de dados
 * @return char* String com os dados do MetaDados no formato de disco
 */
char *to_disk_format_MD(MetaDados *data);

/**
 * @brief Função que imprime os dados do MetaDados
 * 
 * @param data Apontador para a estrutura de dados
 */
void print_MD(MetaDados *data);

/**
 * @brief Função que converte uma string no formato de disco para um buffer que pode ser usado para criar MetaDados
 * 
 * @param data Apontador para a estrutura de dados
 * @return char* String no formato usado na função criar_metaDados
 */
char* from_disk_format_MD(char *data);

/**
 * @brief Função que libera a memória alocada para a estrutura de dados
 * 
 * @param data Apontador para a estrutura de dados
 */
void free_MD(MetaDados *data);

#endif