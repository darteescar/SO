#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DATA* init_data(){
    DATA* data = malloc(sizeof(DATA));
    if(data == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }
    data->title = NULL;
    data->authors = malloc(sizeof(char*));
    if(data->authors == NULL){
        printf("Memory allocation failed\n");
        return NULL;
    }
    data->authors[0] = NULL;
    data->year = 0;
    data->path = NULL;

    return data;
}

void parser_authors(char* authors_str, char** authors){
    if(authors_str == NULL || authors == NULL){
        printf("Invalid authors or authors array\n");
        return;
    }
    
    char* token = strtok(authors_str, ";");
    int i = 0;
    while(token != NULL){
        authors[i] = malloc(strlen(token) + 1);
        if(authors[i] == NULL){
            printf("Memory allocation failed\n");
            return;
        }
        strcpy(authors[i], token);
        token = strtok(NULL, ";");
        i++;
    }
    authors[i] = NULL;
}

void print_data(DATA* data){
    if(data == NULL){
        printf("No data to print\n");
        return;
    }
    printf("Title: %s\n", data->title);
    printf("Authors: ");
    for(int i = 0; data->authors[i] != NULL; i++){
        printf("%s ", data->authors[i]);
    }
    printf("\nYear: %d\n", data->year);
    printf("Path: %s\n", data->path);
}