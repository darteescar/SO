#include "client/services.h"

#define SERVER_FIFO "server_fifo"

void reply(Message *msg){
    switch (get_message_command(msg)) {
        case 'a':
            // Criar o FIFO
            int pidA = getpid();
            char bufferA[512];
            sprintf(bufferA, "tmp/%d", pidA);

            int fifoA = open(bufferA, O_RDONLY);
            if (fifoA == -1) {
                perror("open");
                return;
            }

            //Ler a resposta do servidor
            int keyA;
            read(fifoA, &keyA, sizeof(int));

            char key_str[12];
            sprintf(key_str, "%d", keyA);

            write(1, "Documento ", 10);
            write(1, key_str, strlen(key_str));
            write(1, " indexado\n", 10);
            
            close(fifoA);     
            return;
        case 'c':
            // Criar o FIFO
            int pidC = getpid();
            char bufferC[512];
            sprintf(bufferC, "tmp/%d", pidC);

            int fifoC = open(bufferC, O_RDONLY);
            if (fifoC == -1) {
                perror("open");
                return;
            }
            //Ler a resposta do servidor

            char respostC[500];
            read(fifoC, respostC, sizeof(char)*500);
            printf("%s\n", respostC);
            close(fifoC);


            // Consultar
            
            return;
        case 'd':
            int pidD = getpid();
            char bufferD[512];
            sprintf(bufferD, "tmp/%d", pidD);

            int fifoD = open(bufferD, O_RDONLY);
            if (fifoD == -1) {
                perror("open");
                return;
            }

            char respostaD[100];
            read(fifoD, respostaD, sizeof(char)*100);
            printf("%s\n", respostaD);
            close(fifoD);
 
             // Apagar
 
            return;
        case 'l':
         
             // Listar
 
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
