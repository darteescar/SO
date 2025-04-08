#include "utils/Metadados.h"

struct metaDados{
     char* titulo;
     char** autores;
     int n_autores;
     int ano;
     char* path;
     char* key;
};

char *generate_key() {//Pode ser mudado
    char *buffer=malloc(512);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    sprintf(buffer, "%d", rand()%1000);
    return strdup(buffer);
}

MetaDados *create_metaDados(Message *msg) {
     char *total = get_message_buffer(msg);
     char *token;
     int field = 0, i = 0;

     MetaDados *data = malloc(sizeof(struct metaDados));
     if (data == NULL) {
         perror("malloc");
         exit(EXIT_FAILURE);
     }

     total+=3;//apontar para o titulo

     while ((token = strsep(&total, " ")) != NULL) {
        switch (field) {
            case 0:
                data->titulo = token;
                break;
            case 1:
                char* token2;
                int size=10;

                data->autores = malloc(size * sizeof(char*));
                if (data->autores == NULL) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }

                while ((token2 = strsep(&token, ";")) != NULL){
                    if(i>=size){
                         char** aux=data->autores;
                         data->autores=realloc(aux,2*size* sizeof(char*));
                         if (data->autores == NULL) {
                             perror("realloc");
                             exit(EXIT_FAILURE);
                         }
                         size*=2;
                    }
                    data->autores[i]=token2;
                    i++;
                }
                data->n_autores=i;
                break;
            case 2:
                data->ano = atoi(token);
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

    data->key = generate_key();
    return data;

}

void free_metaDados(MetaDados *data) {
     if (data != NULL) {
         free(data->titulo);
         for (int i = 0; i<data->n_autores; i++) {
             free(data->autores[i]);
         }
         free(data->autores);
         free(data->path);
         free(data->key);
         free(data);
     }
}

void print_metaDados(MetaDados *data) {
    if (data == NULL) {
        write(1, "MetaDados is NULL\n", 18);
         return;
    }

    write(1, "[MetaDados]\n", 13);
    write(1, "Titulo: ", 8);
    write(1, data->titulo, strlen(data->titulo));
    write(1, "\nAutores: ", 9);
    for (int i = 0; i<data->n_autores; i++) {
         write(1, data->autores[i], strlen(data->autores[i]));
         write(1, " ", 1);
    }
    write(1, "\nAno: ", 6);
    char ano[10];
    sprintf(ano, "%d", data->ano);
    write(1, ano, strlen(ano));
    write(1, "\nPath: ", 7);
    write(1, data->path, strlen(data->path));
    write(1, "\nKey: ", 6);
    write(1, data->key, strlen(data->key));
    write(1, "\n\n", 2);
}

char* get_MD_titulo(MetaDados *data){
     return strdup(data->titulo);
}

char* get_MD_path(MetaDados *data){
     return strdup(data->path);
}

int get_MD_ano(MetaDados *data){
     return data->ano;
}

char* get_MD_key(MetaDados *data){
     return strdup(data->key);
}

int get_MD_n_autores(MetaDados *data){
     return data->n_autores;
}

