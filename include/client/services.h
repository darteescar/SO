/**
 * @file services.h
 * @brief Header file for services.c
 * @author Group 37 de SO
 */
#ifndef CLIENT_SERVICES
#define CLIENT_SERVICES

#include "utils/Message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

/**
 * @brief Envia uma mensagem para o servidor.
 * 
 * @param msg Mensagem a enviar.
 * @return Tamanho da mensagem enviada ou -1 em caso de erro.
 */
int send_message(Message *msg);

/**
 * @brief Lê a resposta do servidor e imprime-a.
 */
void reply();


#endif