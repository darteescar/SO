#include "client/services.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void error_message(int option){
    switch(option){
        case 1:
            printf("[TRY] -a <title> <authors> <year> <path>\n");
            break;
        case 2:
            printf("[TRY] -c <key>\n");
            break;
        case 3:
            printf("[TRY] -d <key>\n");
            break;
        case 4:
            printf("[TRY] -l <key> <keyword>\n");
            break;
        case 5:
            printf("[TRY] -s <keyword>\n");
            break;
        case 6:
            printf("[TRY] -f\n");
            break;
        default:
            printf("Unknown error\n");
    }
}

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Invalid entry\n");
        return 1;
    }

    char* option = argv[1];

    if(strcmp(option, "-a") == 0){
        if(argc != 6){
            error_message(1);
            return -1;
        }
        char c = ';';
        int fd = open("myfifo", O_WRONLY);

        write(fd, argv[2], strlen(argv[2]));
        write(fd, &c, sizeof(c));

        write(fd, argv[3], strlen(argv[3]));

        write(fd, &c, sizeof(c));
        write(fd, argv[4], strlen(argv[4]));

        write(fd, &c, sizeof(c));
        write(fd, argv[5], strlen(argv[5]));

        close(fd);
        
        // ADD INDEX
    }else if(strcmp(option, "-c") == 0){
        if(argc != 3){
            error_message(2);
            return -1;
        }


        // CONSULT INDEX
    }else if(strcmp(option, "-d") == 0){
        if(argc != 3){
            error_message(3);
            return -1;
        }

        // DELETE INDEX
    }else if(strcmp(option, "-l") == 0){
        if(argc != 4){
            error_message(4);
            return -1;
        }

        // NUMBER OF LINES OF INDEX WITH KEYWORD
    }else if(strcmp(option, "-s") == 0){
        if(argc != 3){
            error_message(5);
            return -1;
        }

        // LIST ALL INDEXES WITH KEYWORD
    }else if(strcmp(option, "-f") == 0){
        if(argc != 2){
            error_message(6);
            return -1;
        }

        // CLOSE SERVER
    }
    else{
        printf("Invalid option\n");
        return 2;
    }
    return 0;
}