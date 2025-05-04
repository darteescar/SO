/**
 * @file Stack.h
 * @brief Header file for Stack.c
 * @author Group 37 de SO
 */
#ifndef STACK_H
#define STACK_H

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

typedef struct stack Stack;

/**
 * @brief Aloca memória para a estrutura Stack.
 * 
 * @param capacidade_inicial Capacidade inicial da stack.
 * @return Apontador para a estrutura Stack alocada.
 */
Stack* criar_stack(int capacidade_inicial);

/**
 * @brief Expande o tamanho da stack.
 * 
 * @param s Apontador para a estrutura Stack.
 */
void expandir_stack(Stack* s);

/**
 * @brief Adiciona um elemento à stack.
 * 
 * @param s Apontador para a estrutura Stack.
 * @param pos Elemento a ser adicionado.
 */
void push(Stack* s, int pos);

/**
 * @brief Remove um elemento da stack.
 * 
 * @param s Apontador para a estrutura Stack.
 * @return Elemento removido da stack.
 */
int pop(Stack* s);

/**
 * @brief Verifica se a stack está vazia.
 * 
 * @param s Apontador para a estrutura Stack.
 * @return 1 se a stack estiver vazia, 0 caso contrário.
 */
int is_empty(Stack* s);

/**
 * @brief Libera a memória alocada para a stack.
 * 
 * @param s Apontador para a estrutura Stack.
 */
void destruir_stack(Stack* s);

/**
 * @brief Aumenta a capacidade da stack.
 * 
 * @param s Apontador para a estrutura Stack.
 */
void increase_capacity(Stack* s);

#endif