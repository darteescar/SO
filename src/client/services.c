#include "client/services.h"

#define SERVER_FIFO "server_fifo"

void reply(Message *msg){
    switch (get_message_command(msg)) {
        case 'a':
            Client_opcao_A(msg);
            return;

        case 'c':
            Client_opcao_C(msg);
            return;

        case 'd':
            Client_opcao_D(msg);
            return;
            
        case 'l':
            Client_opcao_L(msg);
            return;

        case 's':
             // Pesquisa
            return;

        case 'f':
             // Encerra o servidor
            return;

        default:
            return;
             // Comando inválido
    }
}

void Client_opcao_A(Message *msg){
    // Criar o FIFO
    int pid = getpid();
    char buffer[512];
    sprintf(buffer, "tmp/%d", pid);

    int fifo = open(buffer, O_RDONLY);
    if (fifo == -1) {
        perror("open");
        return;
    }

    //Ler a resposta do servidor
    int key;
    read(fifo, &key, sizeof(int));

    char key_str[12];
    sprintf(key_str, "%d", key);

    write(1, "Documento ", 10);
    write(1, key_str, strlen(key_str));
    write(1, " indexado\n", 10);
    
    close(fifo);     
    return;
}

void Client_opcao_C(Message *msg){
    // Criar o FIFO
    int pid = getpid();
    char buffer[512];
    sprintf(buffer, "tmp/%d", pid);

    int fifo = open(buffer, O_RDONLY);
    if (fifo == -1) {
        perror("open");
        return;
    }

    //Ler a resposta do servidor
    char resposta[500];
    read(fifo, resposta, sizeof(char)*500);
    printf("%s\n", resposta);
    close(fifo);


    // Consultar
    return;
}

void Client_opcao_D(Message *msg){
    int pid = getpid();
    char buffer[512];
    sprintf(buffer, "tmp/%d", pid);

    int fifo = open(buffer, O_RDONLY);
    if (fifo == -1) {
        perror("open");
        return;
    }

    //Ler a resposta do servidor
    char resposta[100];
    read(fifo, resposta, sizeof(char)*100);
    printf("%s\n", resposta);
    close(fifo);
 
    // Apagar
 
    return;
}

void Client_opcao_L(Message *msg){
    int pid = getpid();
    char buffer[512];
    sprintf(buffer, "tmp/%d", pid);

    int fifo = open(buffer, O_RDONLY);
    if (fifo == -1) {
        perror("open");
        return;
    }

    //Ler a resposta do servidor
    char resposta[100];
    read(fifo, resposta, sizeof(char)*100);
    printf("%s\n", resposta);
    close(fifo);
 
    return;

}