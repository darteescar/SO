#include "utils/Cache.h"

struct cache {
    int n_docs;
    int max_docs;
    char *ocupados;
    int redimensionamentos;
    int next_to_disc;
    int n_total;
    Stack *stack;
    MetaDados **docs;
 };

Cache *create_Cache(int max_docs) {
    Cache *docs = malloc(sizeof(Cache));
    if (docs == NULL) {
        perror("Malloc create_Cache da Cache");
        exit(EXIT_FAILURE);
    }

    /* Aloca o array de ponteiros para MetaDados */
    docs->docs = malloc(max_docs * sizeof(MetaDados *));
    if (docs->docs == NULL) {
        perror("Malloc create_Cache dos MetaDados");
        exit(EXIT_FAILURE);
    }

    /* Inicializa todas as posições com NULL (opcional, mas recomendado) */
    for (int i = 0; i < max_docs; i++) {
        docs->docs[i] = NULL;
    }
    docs->n_docs = 0;
    docs->max_docs = max_docs;
    docs->next_to_disc = 0;
    docs->n_total = 0;
    docs->redimensionamentos = 1;
    docs->stack = criar_stack(max_docs);
    docs->ocupados = malloc(max_docs * sizeof(char));
    if (docs->ocupados == NULL) {
        perror("Malloc create_Cache dos ocupados");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < max_docs; i++) {
        docs->ocupados[i] = LIVRE;
    }
    return docs;
}
 
Cache *add_documento(Cache *docs, Message *data, int *pos_onde_foi_add) {
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
            if (docs->ocupados[docs->next_to_disc] == EM_CACHE) escreve_em_disco(docs, docs->next_to_disc);

            char* buffer = get_message_buffer(data);
            
            send_to_Cache(buffer, docs, docs->next_to_disc + docs->max_docs);
            *pos_onde_foi_add = docs->next_to_disc+docs->max_docs;
            docs->next_to_disc++;
        } else {
            int pos = pop(docs->stack);
            
            escreve_em_disco_from_Stack(docs, data, pos);
            *pos_onde_foi_add = pos;
        }   
    }
    return docs;  // Retorna o novo ponteiro de Cache
}

void escreve_em_disco_from_Stack(Cache *docs,Message* msg, int pos) {
    char *buffer = get_message_buffer(msg);
    MetaDados *doc = criar_metaDados(buffer);
    char *result = to_disk_format(doc);

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
    int size = strlen(result);
    ssize_t bytes_written = write(fd, result, size);
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        return;
    }
     
    close(fd);
    docs->ocupados[pos] = EM_DISCO;
}

void print_Cache (Cache *docs) {
    if (docs == NULL) {
        write(1, "Cache is NULL\n", 15);
        return;
    }
    write(1, "[Cache]\n", 9);
    for (int i = 0; i < docs->max_docs; i++) {
        if (docs->ocupados[i] == EM_CACHE || docs->ocupados[i] == EM_DISCO || docs->ocupados[i] == EM_DISCO_E_CACHE) {
            print_metaDados(docs->docs[i]);
        }
    }
}
 
int get_num_docs(Cache *docs) {
    return docs->n_docs;
}
 
int get_nTotal(Cache *docs) {
    return docs->n_total;
}
 
int get_next_to_disc(Cache *docs) {
    return docs->next_to_disc;
}
 
void inc_next_to_disc(Cache *docs) {
    docs->next_to_disc++;
}
 
int get_max_docs(Cache *docs) {
    return docs->max_docs;
} 

void escreve_em_disco(Cache *docs, int pos) {
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
    char *data = to_disk_format(docs->docs[pos%docs->max_docs]);
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
 
void redimensiona_ocupados(Cache *docs) {
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
 
void redimensionar_auxiliares(Cache *docs) {
    redimensiona_ocupados(docs);
    increase_capacity(docs->stack);
}
 
int remove_documento(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_total) {
        return -1;
    }
    if (docs->ocupados[pos] == EM_CACHE || docs->ocupados[pos] == EM_DISCO || docs->ocupados[pos] == EM_DISCO_E_CACHE) {
        //free_metaDados(&(docs->docs[pos]));
        docs->ocupados[pos] = LIVRE;
        docs->n_docs--;
        push(docs->stack, pos);
        return 1;
    } else {
        return -2;
    }
 }
 
int documento_existe(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_total) {
        return -1;
    }
    else if (docs->ocupados[pos] == LIVRE) {
        return -2;
    } else if (docs->ocupados[pos] == EM_CACHE || docs->ocupados[pos] == EM_DISCO || docs->ocupados[pos] == EM_DISCO_E_CACHE) {
        return 1;
    }
    return 0;
 }
 
MetaDados *get_documento(Cache *docs, int pos) {
    return docs->docs[pos % docs->max_docs];
}

MetaDados *get_anywhere_documento(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_total) {
        return NULL;
    }
    if (docs->ocupados[pos] == EM_CACHE || docs->ocupados[pos] == EM_DISCO_E_CACHE ) {
        return docs->docs[pos % docs->max_docs];
    } else {
        char *data = malloc(512);
        if (data == NULL) {
            perror("Malloc disco_to_cache");
            return NULL;
        }
        int fd = open(SERVER_STORAGE, O_RDONLY);
        if (fd == -1) {
            perror("Open disco_to_cache");
            free(data);
            return NULL;
        }
        int offset = pos * 512; // Supondo que cada documento ocupa 512 bytes
        lseek(fd, offset, SEEK_SET);
        read(fd, data, 512);
        close(fd);
        MetaDados *new = criar_metaDados(desserializa_metaDados(data));
        if (new == NULL) {
            return NULL;
        }
        return new;
    }
}

char* desserializa_metaDados(char *data) {
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
 
void disco_to_cache(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->n_total) {
        return;
    }
    char *data = malloc(512);
    if (data == NULL) {
        perror("Malloc disco_to_cache");
        return;
    }
    int fd = open(SERVER_STORAGE, O_RDONLY);
    if (fd == -1) {
        perror("Open disco_to_cache");
        free(data);
        return;
    }
    int offset = pos * 512; // Supondo que cada documento ocupa 512 bytes
    lseek(fd, offset, SEEK_SET);
    read(fd, data, 512);
    close(fd);

    char *buffer = desserializa_metaDados(data);

    int aux = pos%docs->max_docs; // posicao onde queremos meter na cache
    int aux2 = docs->next_to_disc%docs->max_docs; // proxima posicao a meter em disco

    if (docs->ocupados[aux] == LIVRE) {
        send_to_Cache(buffer, docs, pos);
    } else if (docs->ocupados[aux] == EM_DISCO) {
        // Enviar para o cache
        send_to_Cache(buffer, docs, pos);
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
    docs->ocupados[pos] = EM_DISCO_E_CACHE;

}
 
char get_docs_estado(Cache *docs, int pos) {
    if (docs == NULL) {
        return '0';
    }
    return docs->ocupados[pos];
}

void send_to_Cache(char *buffer, Cache *doc, int i) {
    int pos = i % doc->max_docs;

    /*if (doc->docs[pos] != NULL) {
        free_metaDados(doc->docs[pos]);  // Se já existir, limpa antes
    }*/

    doc->docs[pos] = criar_metaDados(buffer);

    if (doc->ocupados[i] == EM_DISCO) {
        printf("Já está em disco...\n");
        return;
    } else {
        //printf("Adicionando documento %d à cache...\n", i);
        doc->ocupados[i] = EM_CACHE;
        doc->n_docs++;
        doc->n_total++;
    }
}