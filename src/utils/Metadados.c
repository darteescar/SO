#include "utils/Metadados.h"

struct metaDados{
     char* titulo;
     char** autores;
     int n_autores;
     int ano;
     char* path;
     int pos_in_disk;
};

MetaDados *criar_metaDados(char *buffer) {
    MetaDados *data = malloc(sizeof(MetaDados));
    if (data == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *total = buffer + 3;  // Ignorar os 3 primeiros caracteres
    char *token;
    int field = 0;
    while ((token = strsep(&total, FIELD_SEP)) != NULL) {
        switch (field) {
            case 0:
                data->titulo = strdup(token);
                break;
            case 1: {
                char *token2;
                int size = 10;
                int j = 0;
                data->autores = malloc(size * sizeof(char *));
                if (data->autores == NULL) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }

                while ((token2 = strsep(&token, ";")) != NULL) {
                    if (j >= size) {
                        size *= 2;
                        data->autores = realloc(data->autores, size * sizeof(char *));
                        if (data->autores == NULL) {
                            perror("realloc");
                            exit(EXIT_FAILURE);
                        }
                    }
                    data->autores[j++] = strdup(token2);
                }

                data->n_autores = j;
                break;
            }
            case 2:
                data->ano = atoi(token);
                break;
            case 3:
                data->path = strdup(token);
                break;
            case 4:
                data->pos_in_disk = atoi(token);
                break;
            default:
                break;
        }
        field++;
    }


    return data;
}

char* get_MD_path(MetaDados *data) {
    return strdup(data->path);
}

int get_MD_pos_in_disk(MetaDados *data) {
    return data->pos_in_disk;
}

void set_disk_position(MetaDados *data, int pos) {
    data->pos_in_disk = pos;
}

char *MD_toString(MetaDados* data, int key) {
   char *str = malloc(520);
   if (str == NULL) {
       perror("malloc");
       exit(EXIT_FAILURE);
   }
   sprintf(str, "Meta Informação do documento %d:\n\nTitulo: %s\nAutores: ",key, data->titulo);
   for (int i = 0; i < data->n_autores; i++) {
       strcat(str, data->autores[i]);
       if (i < data->n_autores - 1) {
           strcat(str, "; ");
       }
   }
   char buffer[256];
   sprintf(buffer, "\nAno: %d\nPath: %s", data->ano, data->path);
   strcat(str, buffer);
   return str;
}

char *to_disk_format(MetaDados *data) {
    if (data == NULL) {
        return NULL;
    }
    char *str = malloc(520);
    if (str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    sprintf(str, "%s|", data->titulo);
    for (int i = 0; i < data->n_autores; i++) {
        strcat(str, data->autores[i]);
        if (i < data->n_autores - 1) {
            strcat(str, ";");
        }
    }
    char buffer[256];
    sprintf(buffer, "|%d|%s|%d",  data->ano, data->path, data->pos_in_disk);
    strcat(str, buffer);
    return str;
}

void free_metaDados(MetaDados *data) {
    if (data != NULL) {
        free(data->titulo);
        for (int i = 0; i<data->n_autores; i++) {
            free(data->autores[i]);
        }
        free(data->autores);
        free(data->path);
        free(data);
    }
}

void print_metaDados(MetaDados *data) {
   if (data == NULL) {
       write(1, "MetaDados is NULL\n", 18);
       return;
   }
   write(1, "[MetaDados]\n", 12);
   write(1, "Titulo: ", 8);
   write(1, data->titulo, strlen(data->titulo));
   write(1, "\nAutores: ", 9);
   for (int i = 0; i < data->n_autores; i++) {
       write(1, data->autores[i], strlen(data->autores[i]));
       if (i < data->n_autores - 1) {
           write(1, ", ", 2);
       }
   }
   write(1, "\nAno: ", 6);
   char buffer[10];
   sprintf(buffer, "%d", data->ano);
   write(1, buffer, strlen(buffer));
   write(1, "\nPath: ", 7);
   write(1, data->path, strlen(data->path));
   write(1, "\n", 1);
}

char* from_disk_format(char *data) {
    char *copy = strdup(data);
    char *aux = copy;

    char *titulo = strsep(&aux, "|");
    char *autores = strsep(&aux, "|");
    char *ano = strsep(&aux, "|");
    char *path = strsep(&aux, "|");
    char *pos_in_disk = strsep(&aux, "|");

    char *buffer = malloc(520);
    if (buffer == NULL) {
        perror("malloc");
        free(copy);
        return NULL;
    }

    sprintf(buffer, "###%s%s%s%s%s%s%s", titulo, FIELD_SEP, autores, FIELD_SEP, ano, FIELD_SEP, path);
    strcat(buffer, FIELD_SEP);
    strcat(buffer, pos_in_disk);
    free(copy);
    return buffer;
}