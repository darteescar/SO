#include "utils/Stack.h"

struct stack {
     int *positions;
     int top;
     int capacity;
 };

// Inicializa a stack
Stack* criar_stack(int capacidade_inicial) {
    Stack* s = malloc(sizeof(Stack));
    if (s == NULL) {
        perror("Malloc criar_stack");
        exit(EXIT_FAILURE);
    }
    s->positions = malloc(sizeof(int) * capacidade_inicial);
    s->top = -1;
    s->capacity = capacidade_inicial;
    return s;
}
 
// Redimensiona a stack se necessário
void expandir_stack(Stack* s) {
    s->capacity *= 2;
    s->positions = realloc(s->positions, sizeof(int) * s->capacity);
}
 
// Insere uma posição (push)
void push(Stack* s, int pos) {
    if (s->top + 1 >= s->capacity)
        expandir_stack(s);
    s->positions[++s->top] = pos;
}
 
// Remove e devolve a última posição (pop)
int pop(Stack* s) {
    if (s->top == -1)
        return -1; // stack vazia
    return s->positions[s->top--];
}
 
 // Verifica se a stack está vazia
int is_empty(Stack* s) {
    return s->top == -1;
}
 
// Liberta a stack
void destruir_stack(Stack* s) {
    free(s->positions);
    free(s);
}

void increase_capacity(Stack* s) {
    // Duplica a capacidade da stack
    s->capacity *= 2;
    s->positions = realloc(s->positions, sizeof(int) * s->capacity);
    if (s->positions == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
}