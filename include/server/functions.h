/**
 * @file functions.h
 * @brief Header file for functions.c
 * @author Group 37 de SO
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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
#include <signal.h>
#include <sys/wait.h>

#include "utils/Cache.h"
#include "utils/Message.h"
#include "utils/Metadados.h"
#include "cache_holder.h"
#include "disk_writer.h"
#include "utils/average_time_clients.h"

/**
 * @brief Função que verifica se o comando recebido é válido.
 * 
 * @param msg Mensagem recebida.
 * @return 1 se o comando for válido, 0 caso contrário, 2 se for o comando -f.
 */
int verifica_comando (MetaDados *msg);

/**
 * @brief Função que envia uma mensagem de erro para o cliente dependendo do comando onde está o erro.
 * 
 * @param msg Mensagem recebida para saber o comando.
 */
void error_message(MetaDados *msg);

/**
 * @brief Função que envia a mensagem para o CACHE_FIFO.
 * 
 * @param msg Mensagem a enviar.
 */
void send_MSG_to_cache (Message *msg);

#endif