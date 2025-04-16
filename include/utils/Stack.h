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

Stack* criar_stack(int capacidade_inicial);

void expandir_stack(Stack* s);

void push(Stack* s, int pos);

int pop(Stack* s);

int is_empty(Stack* s);

void destruir_stack(Stack* s);

void increase_capacity(Stack* s);

#endif