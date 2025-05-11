/**
 * @file cache_holder.h
 * @brief Header file for options.c
 * @author Group 37 de SO
 */
#ifndef OPTIONS_H
#define OPTIONS_H

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
#include <sys/wait.h>
#include <errno.h>

#include "utils/Cache.h"
#include "utils/Metadados.h"
#include "functions.h"

/**
 * @brief Função que executa o comando -a.
 *        Função que adiciona as informações de um documento.
 * 
 * @param msg MetaDados.
 * @param docs Cache onde estão as meta-informações.
 * @return Cache atualizada.
 */
Cache *Server_opcao_A(MetaDados *msg, Cache *docs);

/**
 * @brief Função que executa o comando -c.
 *        Função que consulta as informações de um documento.
 * 
 * @param msg MetaDados.
 * @param docs Cache onde estão as meta-informações.
 */
void Server_opcao_C(MetaDados *msg, Cache *docs);

/**
 * @brief Função que executa o comando -d.
 *        Função que apaga as informações de um documento.
 * 
 * @param msg MetaDados.
 * @param docs Cache onde estão as meta-informações.
 * @return Cache atualizada.
 */
Cache *Server_opcao_D(MetaDados *msg, Cache *docs);

/**
 * @brief Função que executa o comando -l.
 *        Função que cita o número de vezes que uma palavra aparece em um certo documento.
 * 
 * @param msg MetaDados.
 * @param docs Cache onde estão as meta-informações.
 * @param folder Pasta onde estão os documentos.
 */
void Server_opcao_L(MetaDados *msg, Cache *docs, char* folder);

/**
 * @brief Função que executa o comando -s.
 *        Função que cita as keys dos documentos onde aparece uma certa palavra.
 * 
 * @param msg MetaDados.
 * @param docs Cache onde estão as meta-informações.
 * @param folder Pasta onde estão os documentos.
 */
void Server_opcao_S(MetaDados *msg, Cache *docs, char* folder);

/**
 * @brief Função que executa o comando -b.
 *        Função que recupera o backup.
 * 
 * @param msg MetaDados.
 * @param docs Cache onde estão as meta-informações.
 * @return Nova Cache após backup.
 */
Cache* Server_opcao_B(MetaDados *msg, Cache *docs);

/**
 * @brief Função que executa o comando -f.
 *        Função que termina o servidor.
 * 
 * @param msg MetaDados.
 * @param docs Cache onde estão as meta-informações.
 */
void Server_opcao_F(MetaDados *msg, Cache *docs);

/**
 * @brief Função que envia a resposta ao cliente.
 * 
 * @param msg Mensagem a enviar.
 * @param msg_cliente Mensagem do cliente.
 */
void envia_resposta_cliente(const char *msg, MetaDados *msg_cliente);

#endif