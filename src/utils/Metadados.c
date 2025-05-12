#include "utils/Metadados.h"

struct metaDados{
    // INFORMAÇÕES DO DISCO
    char titulo[MAX_TITULO];
    char autores[MAX_AUTORES];
    int n_autores;
    int ano;
    char path[MAX_PATH];
    int pos_in_disk;
    // INFORMAÇÕES DO CLIENTE
    char buffer[512];
    int argc;
    int pid;
    // INFORMAÇÕES DO SERVIDOR
    char flag;
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

    data->buffer[0] = '\0';
    data->pid = -1;
    data->argc = 0;

    data->flag = DISK_INFO_NOT_CREATED;

    return data;
}

void add_MD_info_client (MetaDados *msg, char **argv, int argc, int pid) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    msg->pid = pid;
    msg->argc = argc;
    msg->flag = DISK_INFO_NOT_CREATED;
    
    for (int i = 1; i < argc; i++) {
        strcat(msg->buffer, argv[i]);
        if (i < argc - 1) {  // só se não for o último CUIDADO AQUI OU É -1 OU -2
            strcat(msg->buffer, FIELD_SEP);
        }
    }
}

void add_MD_info_server (MetaDados *msg) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    char *total = get_MD_buffer(msg);  // Ignora os 3 primeiros caracteres
    total += 3;  // Ignora os 3 primeiros caracteres
    char *token;
    int field = 0;

    msg->pos_in_disk = -1;
    msg->flag = DISK_INFO_CREATED;

    while ((token = strsep(&total, FIELD_SEP)) != NULL) {
        switch (field) {
            case 0:
                strncpy(msg->titulo, token, MAX_TITULO - 1);
                msg->titulo[MAX_TITULO - 1] = '\0';
                break;

            case 1:
                strncpy(msg->autores, token, MAX_AUTORES - 1);
                msg->autores[MAX_AUTORES - 1] = '\0';

                // Contar nº de autores (ocorrências de ';' + 1)
                msg->n_autores = 1;
                for (int i = 0; msg->autores[i] != '\0'; i++) {
                    if (msg->autores[i] == ';') {
                        msg->n_autores++;
                    }
                }
                break;

            case 2:
                msg->ano = atoi(token);
                break;

            case 3:
                strncpy(msg->path, token, MAX_PATH - 1);
                msg->path[MAX_PATH - 1] = '\0';
                break;

            case 4:
                msg->pos_in_disk = atoi(token);
                break;

            default:
                break;
        }
        field++;
    }
}

MetaDados *criar_metaDados(char *buffer) {
    MetaDados *data = init_MD();
    if (data == NULL) {
        perror("malloc na criar_metaDados");
        exit(EXIT_FAILURE);
    }

    char *total = buffer + 3;  // Ignora os 3 primeiros caracteres
    char *token;
    int field = 0;

    data->flag = DISK_INFO_CREATED;

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
    return data;
}

MetaDados* desserializa_MetaDados(int pos) {
    char *data = malloc(520);
    if (data == NULL) {
        perror("Malloc desserializa_MetaDados");
        return NULL;
    }
    int fd = open(SERVER_STORAGE, O_RDONLY);
    if (fd == -1) {
        perror("Open desserializa_MetaDados");
        free(data);
        return NULL;
    }
    int offset = pos * 520;
    lseek(fd, offset, SEEK_SET);
    read(fd, data, 520);
    close(fd);
    char *buffer = from_disk_format_MD(data);
    MetaDados *new = criar_metaDados(buffer);
    free(buffer);
    return new;
}

// ------------------------------------------------GETTERS/SETTERS-----------------------------------------------------

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

char *get_MD_buffer(MetaDados *msg) {
    if (msg == NULL) {
        perror("Message is NULL");
        exit(EXIT_FAILURE);
    }
    return strdup(msg->buffer);
}

int get_MD_argc(MetaDados *msg) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    return msg->argc;
}

int get_MD_pid(MetaDados *msg) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    return msg->pid;
}

char get_MD_flag(MetaDados *msg) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    return msg->flag;
}

int get_MD_size (MetaDados *data) {
    return sizeof(MetaDados);
}

char get_MD_command(MetaDados *msg) {
    if (msg == NULL) {
        perror("Message is NULL");
        exit(EXIT_FAILURE);
    }
    return msg->buffer[1];
}

char *get_MD_something (MetaDados *msg, int n) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    char *buffer = get_MD_buffer(msg);
    buffer+=3; // skip comando
    char *token;
    for (int i = 0; i < n; i++) {
        token = strsep(&buffer, FIELD_SEP);
        if (token == NULL) {
            perror("strsep");
            return NULL;
        }
    }
    return token;
}

void set_MD_disk_position(MetaDados *data, int pos) {
    data->pos_in_disk = pos;
}

void set_MD_buffer(MetaDados *msg, char *buffer) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    strncpy(msg->buffer, buffer, sizeof(msg->buffer) - 1);
    msg->buffer[sizeof(msg->buffer) - 1] = '\0';
}

void set_MD_argc(MetaDados *msg, int argc) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    msg->argc = argc;
}

void set_MD_pid(MetaDados *msg, int pid) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    msg->pid = pid;
}

void set_MD_flag(MetaDados *msg, int flag) {
    if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
    }
    msg->flag = flag;
}

// -----------------------------------------------FUNÇÕES AUXILIARES-----------------------------------------------------

char *MD_toString(MetaDados* data, int key) {
    char *str = malloc(512+80);
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

    char *str = malloc(520);
    if (str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    snprintf(str, 520, "%s|%s|%d|%s|%d",
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

    printf("MetaDados:\n");
    printf("Titulo: %s\n", data->titulo);
    printf("Autores: %s\n", data->autores);
    printf("Número de autores: %d\n", data->n_autores);
    printf("Ano: %d\n", data->ano);
    printf("Path: %s\n", data->path);
    printf("Posição no disco: %d\n", data->pos_in_disk);
    printf("Buffer: %s\n", data->buffer);
    printf("Argc: %d\n", data->argc);
    printf("PID: %d\n", data->pid);
    printf("====================================\n");
    printf("\n");

    
}

char* from_disk_format_MD(char *data) {
    char *copy = strdup(data);
    char *aux = copy;

    char *titulo = strsep(&aux, "|");
    char *autores = strsep(&aux, "|");
    char *ano = strsep(&aux, "|");
    char *path = strsep(&aux, "|");
    char *pos_in_disk = strsep(&aux, "|");

    char *buffer = malloc(strlen(titulo) + strlen(autores) + strlen(ano) + strlen(path) + strlen(pos_in_disk) + 10);
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

void free_MD(MetaDados *data) {
    if (data != NULL) {
        free(data);
    }   
}