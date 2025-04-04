#include "server/functions.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

    printf("[SERVER STARTED]\n");
    int o = mkfifo("myfifo", 0666);
    if (o==-1){
        perror("mkfifo");
        return 1;
    } else {
        printf("FIFO 1 created\n");
    }
    int fd = open("myfifo", O_RDONLY);

    DATA* data = init_data();
    if (data == NULL) {
        perror("init_data");
        return 1;
    }
    data->title = malloc(100);
    if (data->title == NULL) {
        perror("malloc");
        free(data);
        return 1;
    }
    data->authors = malloc(100 * sizeof(char*));
    if (data->authors == NULL) {
        perror("malloc");
        free(data->title);
        free(data);
        return 1;
    }
    for (int i = 0; i < 100; i++) {
        data->authors[i] = malloc(100);
        if (data->authors[i] == NULL) {
            perror("malloc");
            for (int j = 0; j < i; j++) {
                free(data->authors[j]);
            }
            free(data->authors);
            free(data->title);
            free(data);
            return 1;
        }
    }
    data->year = 0;
    data->path = malloc(100);
    if (data->path == NULL) {
        perror("malloc");
        for (int i = 0; i < 100; i++) {
            free(data->authors[i]);
        }
        free(data->authors);
        free(data->title);
        free(data);
        return 1;
    }

    char *total;
    total = malloc(100);
    read(fd, total, 100);
    char* token;
    int field = 0;

    while ((token = strsep(&total, ";")) != NULL) {
        switch (field) {
            case 0:
                data->title = token;
                break;
            case 1:
                parser_authors(token, data->authors);
                break;
            case 2:
                data->year = atoi(token);
                break;
            case 3:
                data->path = token;
                break;
            default:
                printf("Unknown field\n");
                break;
        }
        field++;
    }

    close(fd);
    print_data(data);
    
    return 0;
}