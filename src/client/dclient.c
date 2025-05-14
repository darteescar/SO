#include "client/services.h"

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

    MetaDados *msg = init_MD();

    int pid = getpid();

    add_MD_info_client(msg, argv, argc, pid);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    char path[24];
    sprintf(path, "tmp/%d", get_MD_pid(msg));

    int d = mkfifo(path, 0600);
    if (d == -1) {
            perror("mkfifo na main do dclient");
            return -1;
    }

    // Enviar a mensagem para o servidor
    int x = send_message(msg);

    if (x == -1) {
        perror("Send_message na main do dclient");
        free_MD(msg);
        return -1;
    } else {
        free_MD(msg);
    }

    // Ler a resposta do servidor
    reply();

    // Verifica o tempo decorrido
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long micros = end.tv_usec - start.tv_usec;
    double elapsed = seconds + micros*1e-6;

    registar_desempenho(elapsed);

    return 0;
}