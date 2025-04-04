#ifndef UTILS_H
#define UTILS_H

#include "client/services.h"
#include "server/functions.h"

typedef struct data{
    char* title;
    char** authors;
    int year;
    char* path;
}DATA;

DATA* init_data();

void parser_authors(char* authors_str, char** authors);

void print_data(DATA* data);

#endif