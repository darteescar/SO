#include "utils/Metadados.h"

struct metaDados{
     char* titulo;
     char** autores;
     int n_autores;
     int ano;
     char* path;
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

char *MD_toString(MetaDados* data, int key) {
   char *str = malloc(1000);
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
    char *str = malloc(512);
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
    sprintf(buffer, "|%d|%s",  data->ano, data->path);
    strcat(str, buffer);
    return str;
}

void escreve_em_disco(Documentos *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_docs) {
        return;
    }

    //printf("Escrevendo em disco...\n");
    int fd = open(SERVER_STORAGE, O_WRONLY | O_CREAT , 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    // Escrever os dados do documento no disco
    int offset = pos * 512; // Supondo que cada documento ocupa 512 bytes (caracteres de separação tidos em conta)
    int x = lseek(fd, offset, SEEK_SET);
    if (x == -1) {
        perror("lseek");
        close(fd);
        return;
    }
    char *data = serializa_metaDados(&(docs->docs[pos%docs->max_docs]));
    int size = strlen(data);
    ssize_t bytes_written = write(fd, data, size);
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        return;
    }
    
    close(fd);
    docs->ocupados[pos] = EM_DISCO;
}

void redimensiona_ocupados(Documentos *docs) {
    //printf("Redimensionando ocupados...\n");
    if (docs == NULL) {
        return;
    }

    int old_size = docs->max_docs * docs->redimensionamentos;

    docs->redimensionamentos *= 2;

    int new_size = docs->max_docs * docs->redimensionamentos;
    //printf("Old size: %d, New size: %d\n", old_size, new_size);

    char *new_ocupados = realloc(docs->ocupados, new_size * sizeof(char));
    if (new_ocupados == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    docs->ocupados = new_ocupados;
    for (int i = old_size; i < new_size; i++) {
        docs->ocupados[i] = LIVRE;
    }
}

void redimensionar_auxiliares(Documentos *docs) {
    redimensiona_ocupados(docs);
    increase_capacity(docs->stack);
}

Documentos *add_documento(Documentos *docs, Message *data, int *pos_onde_foi_add) {
    if (docs->n_docs < docs->max_docs) {
        // Encontrar índice livre
        int i = 0;
        while (docs->ocupados[i] != LIVRE) i++;
        char* buffer = get_message_buffer(data);

        send_to_Cache(buffer, docs, i);
        *pos_onde_foi_add = i;
        
    } else {
        //printf("N-Docs: %d\n", docs->n_docs);
        if (docs->n_docs >= docs->max_docs * docs->redimensionamentos) redimensionar_auxiliares(docs);

        if (is_empty(docs->stack)) {
            if (docs->ocupados[docs->next_to_disc]==EM_CACHE) escreve_em_disco(docs, docs->next_to_disc);

            char* buffer = get_message_buffer(data);
            
            send_to_Cache(buffer, docs, docs->next_to_disc + docs->max_docs);
            *pos_onde_foi_add = docs->next_to_disc+docs->max_docs;
            docs->next_to_disc++;
        } else {
            int pos = pop(docs->stack);
            char* buffer = get_message_buffer(data);
            send_to_Cache(buffer, docs, pos);
            *pos_onde_foi_add = pos;
        }
        
    }

    return docs;  // Retorna o novo ponteiro de documentos
}

int remove_documento(Documentos *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_total) {
        return -1;
    }
    if (docs->ocupados[pos] == EM_CACHE || docs->ocupados[pos] == EM_DISCO) {
        //free_metaDados(&(docs->docs[pos]));
        docs->ocupados[pos] = LIVRE;
        docs->n_docs--;
        push(docs->stack, pos);
        return 1;
    } else {
        return -2;
    }
}

int documento_existe(Documentos *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_total) {
        return -1;
    }
    else if (docs->ocupados[pos] == LIVRE) {
        return -2;
    } else if (docs->ocupados[pos] == EM_CACHE || docs->ocupados[pos] == EM_DISCO) {
        return 1;
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