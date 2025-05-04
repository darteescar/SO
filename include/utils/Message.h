/**
 * @file Message.h
 * @brief Header file for Message.c
 * @author Group 37 de SO
 */
#ifndef MESSAGE_H
#define MESSAGE_H

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

#define FIELD_SEP "\x1E"

typedef struct message Message;

/**
 * @brief Aloca memória para a estrutura Message.
 * 
 * @return Apontador para a estrutura Message alocada.
 */
Message *init_message();

/**
 * @brief Cria uma mensagem a partir dos argumentos passados.
 * 
 * @param msg Apontador para a estrutura Message.
 * @param argv Argumentos passados.
 * @param argc Número de argumentos.
 * @param pid PID do processo cliente que cria a mensagem.
 */
void create_message(Message *msg, char *argv[], int argc, int pid);

/**
 * @brief Retorna o tamanho da mensagem.
 * 
 * @param msg Apontador para a estrutura Message.
 * @return Tamanho da mensagem.
 */
size_t get_message_size(Message *msg);

/**
 * @brief Libera a memória alocada para a mensagem.
 * 
 * @param msg Apontador para a estrutura Message.
 */
void free_message(Message *msg);

/**
 * @brief Imprime a mensagem.
 * 
 * @param msg Apontador para a estrutura Message.
 */
void print_message(Message *msg);

/**
 * @brief Retorna o comando no buffer da mensagem.
 * 
 * @param msg Apontador para a estrutura Message.
 * @return Comando da mensagem.
 */
char get_message_command(Message *msg);

/**
 * @brief Retorna o campo argc da mensagem.
 * 
 * @param msg Apontador para a estrutura Message.
 * @return Campo argc.
 */
int get_message_argc(Message *msg);

/**
 * @brief Retorna o campo buffer da mensagem.
 * 
 * @param msg Apontador para a estrutura Message.
 * @return Campo buffer da mensagem.
 */
char* get_message_buffer(Message *msg);

/**
 * @brief Retorna o campo pid da mensagem.
 * 
 * @param msg Apontador para a estrutura Message.
 * @return Campo pid da mensagem.
 */
int get_message_pid(Message *msg);

#endif