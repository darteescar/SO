#include "utils/Metadados.h"

struct metaDados{
     char* titulo;
     char** autores;
     int n_autores;
     int ano;
     char* path;
};

struct documentos {
    int n_docs;
    int max_docs;
    int *ocupados;
    MetaDados docs[];
};

void create_metaDados(Message *msg, Documentos *doc, int i) {
    char *total = get_message_buffer(msg);
    char *token;
    int field = 0;

    MetaDados *data = &(doc->docs[i]);

    total += 3; // Skip prefixo

    while ((token = strsep(&total, "+")) != NULL) {
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
                    data->autores[j] = strdup(token2);
                    j++;
                }
                data->n_autores = j;
                break;
            }
            case 2:
                data->ano = atoi(token);
                break;
            case 3:
                char *path = malloc(strlen(PATH) + strlen(token) + 1);
                if (path == NULL) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                strcpy(path, PATH);
                strcat(path, token);
                data->path = path;
                break;
            default:
                printf("Unknown field\n");
                break;
        }
        field++;
    }

    doc->ocupados[i] = 1;
    doc->n_docs++;
}

void free_metaDados(MetaDados *data) {
     if (data != NULL) {
         free(data->titulo);
         for (int i = 0; i<data->n_autores; i++) {
             free(data->autores[i]);
         }
         free(data->autores);
         free(data->path);
         //free(data);
     }
}

void print_metaDados(MetaDados data) {
    write(1, "[MetaDados]\n", 13);
    write(1, "Titulo: ", 8);
    write(1, data.titulo, strlen(data.titulo));
    write(1, "\nAutores: ", 9);
    for (int i = 0; i<data.n_autores; i++) {
         write(1, data.autores[i], strlen(data.autores[i]));
         write(1, " ", 1);
    }
    write(1, "\nAno: ", 6);
    char ano[10];
    sprintf(ano, "%d", data.ano);
    write(1, ano, strlen(ano));
    write(1, "\nPath: ", 7);
    write(1, data.path, strlen(data.path));
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

int get_MD_n_autores(MetaDados *data){
     return data->n_autores;
}

char** get_MD_autores(MetaDados *data){
     return data->autores;
}

char *MD_toString(MetaDados* data, int key){
    char *str = malloc(1000);
    if (str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    sprintf(str, "Meta Informação do documento %d:\n\nTitulo: %s\nAutores: ",key, data->titulo);
    for (int i = 0; i < data->n_autores; i++) {
        strcat(str, data->autores[i]);
        if (i < data->n_autores - 1) {
            strcat(str, ", ");
        }
    }
    char buffer[256];
    sprintf(buffer, "\nAno: %d\nPath: %s", data->ano, data->path);
    strcat(str, buffer);
    return str;
}

Documentos *create_documentos(int max_docs) {
    Documentos *docs = malloc(sizeof(Documentos) + max_docs * sizeof(MetaDados));
    if (docs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    docs->n_docs = 0;
    docs->max_docs = max_docs;
    docs->ocupados = malloc(max_docs * sizeof(int));
    if (docs->ocupados == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < max_docs; i++) {
        docs->ocupados[i] = 0;
    }
    return docs;
}

Documentos *add_documento(Documentos *docs, Message *data, int *pos_onde_foi_add) {
    if (docs->n_docs < docs->max_docs) {
        // Encontrar índice livre
        int i = 0;
        while (docs->ocupados[i] == 1) i++;
        //printf("a tentar adicionar na posicao %d\n", i);
        create_metaDados(data, docs, i);
        //printf("adicionado na posicao %d\n", i);
        *pos_onde_foi_add = i;
    } else {
        
        // Se não houver espaço, aumentar o tamanho do array
        int new_max_docs = docs->max_docs * 2;
        Documentos *new_docs = realloc(docs, sizeof(Documentos) + new_max_docs * sizeof(MetaDados));
        if (new_docs == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        docs = new_docs;
        docs->max_docs = new_max_docs;
        docs->ocupados = realloc(docs->ocupados, new_max_docs * sizeof(int));
        if (docs->ocupados == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        for (int i = docs->max_docs / 2; i < new_max_docs; i++) {
            docs->ocupados[i] = 0;
        }
        

        // Encontrar índice livre
        int i = 0;
        while (docs->ocupados[i] == 1) i++;
        //printf("a tentar adicionar na posicao %d\n", i);
        create_metaDados(data, docs, i);
        //printf("adicionado na posicao %d\n", i);
        *pos_onde_foi_add = i;

    }

    return docs;  // Retorna o novo ponteiro de documentos
}

int remove_documento(Documentos *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->max_docs) {
        return -1;
    }
    if (docs->ocupados[pos] == 1) {
        free_metaDados(&(docs->docs[pos]));
        docs->ocupados[pos] = 0;
        docs->n_docs--;
        return 1;
    } else {
        return -2;
    }
}

int documento_existe(Documentos *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->max_docs) {
        return -1;
    }
    else if (docs->ocupados[pos] == 0) {
        return -2;
    } else if (docs->ocupados[pos] == 1) {
        return 1;
    }
    return 0;
}

MetaDados *get_documento (Documentos *docs, int pos) {
    return &(docs->docs[pos]);
}

void print_documentos (Documentos *docs) {
    if (docs == NULL) {
        write(1, "Documentos is NULL\n", 19);
        return;
    }
    write(1, "[Documentos]\n", 14);
    for (int i = 0; i < docs->n_docs; i++) {
        if (docs->ocupados[i] == 1) {
            print_metaDados(docs->docs[i]);
        }
    }
}