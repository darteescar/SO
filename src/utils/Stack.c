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

int existe_MapPos_stack(Stack* s, int pos, int capacity) {
    int found = pos%capacity;
    for (int i = 0; i <= s->top; i++) {
        if (s->positions[i]%capacity == found) {
            return s->positions[i]; // A posição existe na stack
        }
    }
    return -1; // A posição não existe na stack
}

void remove_pos_stack(Stack* s, int pos) {
    for (int i = 0; i <= s->top; i++) {
        if (s->positions[i] == pos) {
            // Move todos os elementos acima para baixo
            for (int j = i; j < s->top; j++) {
                s->positions[j] = s->positions[j + 1];
            }
            s->top--;
            return;
        }
    }
}

void print_stack(Stack* s) {
    printf("Stack: ");
    for (int i = 0; i <= s->top; i++) {
        printf("N%d -> %d, ", i,  s->positions[i]);
    }
    printf("\n");
}