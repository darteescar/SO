#include "utils/Metadados.h"

#define MAX_TITULO 512
#define MAX_AUTORES 512
#define MAX_PATH 10

struct metaDados{
    char titulo[MAX_TITULO];
    char autores[MAX_AUTORES];
    int n_autores;
    int ano;
    char path[MAX_PATH];
    int pos_in_disk;
};

MetaDados *init_MD() {
    MetaDados *data = malloc(sizeof(MetaDados));
    if (data == NULL) {
        perror("malloc na init_MD");
        exit(EXIT_FAILURE);
    }
    data->titulo[0] = '\0';
    data->autores[0] = '\0';
    data->n_autores = 0;
    data->ano = 0;
    data->path[0] = '\0';
    data->pos_in_disk = -1;
    return data;
}

MetaDados *criar_metaDados(char *buffer) {
    MetaDados *data = malloc(sizeof(MetaDados));
    if (data == NULL) {
        perror("malloc na criar_metaDados");
        exit(EXIT_FAILURE);
    }

    char *total = buffer + 3;  // Ignora os 3 primeiros caracteres
    char *token;
    int field = 0;

    data->pos_in_disk = -1;

    while ((token = strsep(&total, FIELD_SEP)) != NULL) {
        switch (field) {
            case 0:
                strncpy(data->titulo, token, MAX_TITULO - 1);
                data->titulo[MAX_TITULO - 1] = '\0';
                break;

            case 1:
                strncpy(data->autores, token, MAX_AUTORES - 1);
                data->autores[MAX_AUTORES - 1] = '\0';

                // Contar nº de autores (ocorrências de ';' + 1)
                data->n_autores = 1;
                for (int i = 0; data->autores[i] != '\0'; i++) {
                    if (data->autores[i] == ';') {
                        data->n_autores++;
                    }
                }
                break;

            case 2:
                data->ano = atoi(token);
                break;

            case 3:
                strncpy(data->path, token, MAX_PATH - 1);
                data->path[MAX_PATH - 1] = '\0';
                break;

            case 4:
                data->pos_in_disk = atoi(token);
                break;

            default:
                break;
        }
        field++;
    }

    print_MD(data);

    return data;
}

char* get_MD_path(MetaDados *data) {
    if (data == NULL) {
        return NULL;
    }
    char *path = malloc(strlen(data->path) + 1);
    if (path == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(path, data->path);
    return path;

}

int get_MD_pos_in_disk(MetaDados *data) {
    return data->pos_in_disk;
}

void set_MD_disk_position(MetaDados *data, int pos) {
    data->pos_in_disk = pos;
}

char *MD_toString(MetaDados* data, int key) {
    char *str = malloc(1108);
    if (str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    sprintf(str, "Meta Informação do documento %d:\n\nTitulo: %s\nAutores: %s\nAno: %d\nPath: %s\n",
            key, data->titulo, data->autores, data->ano, data->path);
    return str;
}

char *to_disk_format_MD(MetaDados *data) {
    if (data == NULL) return NULL;

    char *str = malloc(6000);
    if (str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    snprintf(str, 1108, "%s|%s|%d|%s|%d",
             data->titulo,
             data->autores,
             data->ano,
             data->path,
             data->pos_in_disk);

    return str;
}

void print_MD(MetaDados *data) {
    if (data == NULL) {
        write(1, "MetaDados is NULL\n", 18);
        return;
    }

    dprintf(1,
        "[MetaDados]\nTitulo: %s\nAutores: %s\nAno: %d\nPath: %s\nPos in Disk: %d\n",
        data->titulo, data->autores, data->ano, data->path, data->pos_in_disk);
}

char* from_disk_format_MD(char *data) {
    char *copy = strdup(data);
    char *aux = copy;

    char *titulo = strsep(&aux, "|");
    char *autores = strsep(&aux, "|");
    char *ano = strsep(&aux, "|");
    char *path = strsep(&aux, "|");
    char *pos_in_disk = strsep(&aux, "|");

    char *buffer = malloc(1108);
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

int get_MD_size (MetaDados *data) {
    if (data == NULL) {
        return -1;
    }
    int x = sizeof(data->titulo) + sizeof(data->autores) + sizeof(data->ano) + sizeof(data->path); 
    printf("Tamanho do metadados: %d\n", x);
    return x;
}

void write_MD (MetaDados *data, int fd) {
    if (data == NULL) {
        return;
    }
    write(fd, data, sizeof(MetaDados));
}

MetaDados *read_MD (int fd) {
    MetaDados *data = malloc(sizeof(MetaDados));
    if (data == NULL) {
        perror("malloc");
        return NULL;
    }
    read(fd, data, sizeof(MetaDados));
    return data;
}