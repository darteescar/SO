#include "client/services.h"
#include "utils/Message.h"

#define SERVER_FIFO "tmp/server_fifo"

/**
 * @brief Função principal do cliente.
 * 
 * @param argc Número de argumentos.
 * @param argv Argumentos passados para o programa.
 */
int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Invalid entry\n");
        return -1;
    }

    Message *msg = init_message();

    int pid = getpid();

    create_message(msg, argv, argc, pid);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    // Enviar a mensagem para o servidor
    int x = send_message(msg);

    if (x == -1) {
        perror("Send_message na main do dclient");
        free_message(msg);
        return -1;
    } else {
        free_message(msg);
    }

    // Ler a resposta do servidor
    reply();

    // Verifica o tempo decorrido
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long micros = end.tv_usec - start.tv_usec;
    double elapsed = seconds + micros*1e-6;

    printf("\nTempo decorrido: %.6f segundos\n", elapsed);

    FILE *f = fopen("times.txt", "a");
    if (f == NULL) {
        perror("Erro ao abrir times.txt");
        return -1;
    }
    fprintf(f, "%.6f\n", elapsed);
    fclose(f);


    return 0;
}