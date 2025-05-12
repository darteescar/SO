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
#include "utils/Metadados.h"
#include "options.h"
#include "disk_writer.h"
#include "utils/average_time_clients.h"

#define SERVER_FIFO "tmp/server_fifo"
#define CACHE_FIFO "tmp/cache_fifo"

#define DISK_INFO_CREATED 'a'
#define DISK_INFO_NOT_CREATED 'b'


/**
 * @brief Função que processa o pedido do cliente.
 * 
 * @param mt Mensagem recebida.
 * @param cache Cache onde estão as meta-informações.
 * @param server_down Flag que indica se o servidor vai terminar.
 * @param folder Pasta onde estão os documentos.
 * @return Cache atualizada.
 */
Cache *process_message(MetaDados *mt, Cache *cache, int *server_down, char *folder);

/**
 * @brief Função que verifica se o comando recebido é válido.
 * 
 * @param msg MetaDados recebida.
 * @return 1 se o comando for válido, 0 caso contrário, 2 se for o comando -f.
 */
int verifica_comando (MetaDados *msg);

/**
 * @brief Função que envia uma mensagem de erro para o cliente dependendo do comando onde está o erro.
 * 
 * @param msg MetaDados recebida para saber o comando.
 */
void error_message(MetaDados *msg);

/**
 * @brief Função que altera os MetaDados de maneira a que o servidor os possa ler e adicionar.
 *        Depois de alterar os MetaDados, envia-os para o servidor.
 * 
 * @param mt MetaDados a alterar e enviar para o servidor.
 */
void send_to_SERVER_again(MetaDados *mt);

/**
 * @brief Função que envia o MetaDados para o SERVER_FIFO.
 * 
 * @param mt MetaDados a enviar.
 */
void send_to_server(MetaDados *mt);

#endif