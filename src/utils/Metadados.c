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
    char *ocupados;
    int redimensionamentos;
    int next_to_disc;
    int n_total;
    Stack *stack;
    MetaDados docs[];
};

/*---------------------------------------------------MetaDados-----------------------------------------------------*/

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
           strcat(str, "; ");
       }
   }
   char buffer[256];
   sprintf(buffer, "\nAno: %d\nPath: %s", data->ano, data->path);
   strcat(str, buffer);
   return str;
}

void send_to_Cache(char* buffer, Documentos *doc, int i) {
    char *total = buffer;
    char *token;
    int field = 0;

    MetaDados *data = &(doc->docs[i%doc->max_docs]);

    total += 3; // Salta prefixo, se necessário (verifica se deves manter isto)

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

    if (doc->ocupados[i] == EM_DISCO) {
        printf("Já está em disco...\n");
        return;
    } else {
        printf("Adicionando documento %d à cache...\n", i);
        doc->ocupados[i] = EM_CACHE;
        doc->n_docs++;
        doc->n_total++;
    }
}

Documentos *create_documentos(int max_docs) {
    Documentos *docs = malloc(sizeof(Documentos) + max_docs * sizeof(MetaDados));
    if (docs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    docs->n_docs = 0;
    docs->max_docs = max_docs;
    docs->next_to_disc = 0;
    docs->n_total = 0;
    docs->redimensionamentos = 1;
    docs->stack = criar_stack(max_docs);
    docs->ocupados = malloc(max_docs * sizeof(char));
    if (docs->ocupados == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < max_docs; i++) {
        docs->ocupados[i] = LIVRE;
    }
    return docs;
}

char *serializa_metaDados(MetaDados *data) {
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
    //printf("Serializando: %s\n", str);
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
            escreve_em_disco(docs, pos);
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
    return 0;
}

MetaDados *get_documento (Documentos *docs, int pos) {
    return &(docs->docs[pos%docs->max_docs]);
}

void print_documentos (Documentos *docs) {
    if (docs == NULL) {
        write(1, "Documentos is NULL\n", 19);
        return;
    }
    write(1, "[Documentos]\n", 14);
    for (int i = 0; i < docs->max_docs; i++) {
        if (docs->ocupados[i] == EM_CACHE || docs->ocupados[i] == EM_DISCO) {
            print_metaDados(&(docs->docs[i]));
        }
    }
}

int get_num_docs(Documentos *docs) {
    return docs->n_docs;
}

int get_nTotal(Documentos *docs) {
    return docs->n_total;
}

int get_next_to_disc(Documentos *docs) {
    return docs->next_to_disc;
}

void inc_next_to_disc(Documentos *docs) {
    docs->next_to_disc++;
}

int get_max_docs(Documentos *docs) {
    return docs->max_docs;
}

char* desserializa_metaDados(char *data) { //transforma a string em cache para uma compativel com a funcao send_to_Cache

    char *titulo = strsep(&data, "|");
    char *autores = strsep(&data, "|");
    char *ano = strsep(&data, "|");
    char *path = strsep(&data, "|");
    char *buffer = malloc(512);
    if (buffer == NULL) {
        perror("malloc");
        return NULL;
    }
    sprintf(buffer, "###%s%s%s%s%s", titulo, FIELD_SEP, autores, FIELD_SEP, ano);
    strcat(buffer, FIELD_SEP);
    strcat(buffer, path);

    return buffer;
    
}

void disco_to_cache(Documentos *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_total) {
        return;
    } else if(docs->ocupados[pos] == EM_DISCO) {
        //printf("Lendo do disco...\n");
        char *data = malloc(512);
        if (data == NULL) {
            perror("malloc");
            return;
        }
        int fd = open(SERVER_STORAGE, O_RDONLY);
        if (fd == -1) {
            perror("open");
            free(data);
            return;
        }
        int offset = pos * 512; // Supondo que cada documento ocupa 512 bytes
        lseek(fd, offset, SEEK_SET);
        read(fd, data, 512);
        close(fd);

        char *buffer = desserializa_metaDados(data);

        int aux = pos%docs->max_docs; //posicao onde queremos meter na cache
        int aux2 = docs->next_to_disc%docs->max_docs; //proxima posicao a meter em disco

        if (docs->ocupados[aux] == LIVRE) {
            send_to_Cache(buffer, docs, pos);
        } else if (docs->ocupados[aux] == EM_DISCO) {
            // Enviar para o cache
            send_to_Cache(buffer, docs, pos);
            return;
        } else {
            if (aux==aux2){//se for a mesma posicao procede-se normalmente
                escreve_em_disco(docs, docs->next_to_disc);
                send_to_Cache(buffer, docs, pos);
                docs->next_to_disc++;
            } else if (aux<aux2){//senao acha-se qual seria o indice do metadados que estaria em aux2 e mandamo-lo para a cache antecipadamente
                escreve_em_disco(docs, (aux2+aux)+docs->next_to_disc);
                send_to_Cache(buffer, docs, pos);
            } else {
                escreve_em_disco(docs, docs->next_to_disc+(aux-aux2));
                send_to_Cache(buffer, docs, pos);
            }
        }
    }
}

char get_docs_estado(Documentos *docs, int pos) {
    if (docs == NULL) {
        return '0';
    }
    return docs->ocupados[pos];
}

