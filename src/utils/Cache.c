#include "utils/Cache.h"

struct cache {
    int dinamica; // 0 se for estatico, 1 se for dinamico
    MetaDados **docs;
    Stack *stack_to_disc;
    Stack *stack_to_cache;
    char *ocupados;
    int size;
    int capacity;
    int redimensionamentos;
    int next_to_disc;
 };

/*
----------------------------------------------------------------------------------------------------------------------
--------------------------------------FUNÇÕES PARA A ABSTRAÇÃO DA CACHE-----------------------------------------------
----------------------------------------------------------------------------------------------------------------------
*/

Cache *create_Cache(int max_docs, int flag) {
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

    docs->size = 0;
    docs->dinamica = flag;
    docs->capacity = max_docs;
    docs->next_to_disc = max_docs;
    docs->redimensionamentos = 1;
    
    docs->stack_to_disc = criar_stack(max_docs);
    docs->stack_to_cache = criar_stack(max_docs);

    docs->ocupados = malloc(max_docs * sizeof(char));
    if (docs->ocupados == NULL) {
        perror("Malloc create_Cache dos ocupados");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < max_docs; i++) {
        docs->ocupados[i] = LIVRE;
        docs->docs[i] = NULL;
    }
    return docs;
}
 
Cache *add_documento(Cache *cache, Message *data, int *pos_onde_foi_add) {
    if (cache->dinamica == 0) {
        return add_documento_Estaticamente(cache, data, pos_onde_foi_add);
    } else {
        return add_documento_Dinamicamente(cache, data, pos_onde_foi_add);
    }
}

Cache *add_documento_Estaticamente(Cache *cache, Message *data, int *pos_onde_foi_add){
    char *buffer = get_message_buffer(data);
    MetaDados *mt = criar_metaDados(buffer);
    free(buffer);

    if (cache->size < cache->capacity) {
        *pos_onde_foi_add = cache->size;
        add_to_Cache(cache, mt, cache->size);
    }
    else {
        if (cache->size >= cache->redimensionamentos*cache->capacity ) redimensionar_auxiliares(cache);

        if (is_empty(cache->stack_to_cache)){
            if (is_empty(cache->stack_to_disc)) {

                add_to_Disk(cache, cache->docs[cache->next_to_disc%cache->capacity]);
                add_to_Cache(cache, mt, cache->next_to_disc);
                *pos_onde_foi_add = cache->next_to_disc;
                cache->next_to_disc++;

            } else {
                int pos = pop(cache->stack_to_disc);
                set_disk_position(mt, pos);
                add_to_Disk(cache, mt);
                *pos_onde_foi_add = pos;
            }
        } else {
            int pos = pop(cache->stack_to_cache);
            add_to_Cache(cache, mt, pos);
            *pos_onde_foi_add = pos;
        } 
    }
    
    return cache;  // Retorna o novo ponteiro de Cache
}

Cache *add_documento_Dinamicamente(Cache *cache, Message *data, int *pos_onde_foi_add){
    char *buffer = get_message_buffer(data);
    MetaDados *mt = criar_metaDados(buffer);
    free(buffer);

    if (cache->size < cache->capacity) {
        int i = 0;
        while (cache->ocupados[i] != LIVRE) i++;
        add_to_Cache(cache, mt, i);
        *pos_onde_foi_add = i;

    } else {
        // Se não houver espaço, aumentar o tamanho do array
        int new_max_docs = cache->capacity * 2;
        MetaDados **new_docs = realloc(cache->docs, new_max_docs * sizeof(MetaDados *));
        if (new_docs == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        char *new_ocupados = realloc(cache->ocupados, new_max_docs * sizeof(char));
        if (new_ocupados == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        cache->ocupados = new_ocupados;
        for (int i = cache->capacity; i < new_max_docs; i++) {
            cache->ocupados[i] = LIVRE;
        }

        cache->docs = new_docs;
        cache->capacity = new_max_docs;

        if (cache->ocupados == NULL) {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        // Encontrar índice livre
        int i = 0;
        while (cache->ocupados[i] != LIVRE) i++;
        char* buffer = get_message_buffer(data);

        add_to_Cache(cache, criar_metaDados(buffer), i);
        free(buffer);
        *pos_onde_foi_add = i;

    }

    return cache;

}

void add_to_Cache(Cache *cache, MetaDados *data, int pos) {
    if (get_MD_pos_in_disk(data) == -1) {
        //printf("Adicionado à cache na posição %d\n", pos);
        set_disk_position(data, pos);
    }
    cache->docs[pos%cache->capacity] = data;
    cache->ocupados[pos] = EM_CACHE;
    cache->size++;
    //printf("Tamanho da cache: %d\n", cache->size);
}

void add_to_Disk(Cache *cache, MetaDados *data) {
    int pos = get_MD_pos_in_disk(data);
    //printf("Adicionado ao disco na posição %d\n", pos);

    cache->ocupados[pos] = EM_DISCO;

    //printf("Adicionando ao disco...\n");

    int fd = open(SERVER_STORAGE, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }

    int offset = pos * 520;
    if (lseek(fd, offset, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        return;
    }

    char *buffer = to_disk_format(data);
    if (write(fd, buffer, strlen(buffer)) == -1) {
        perror("write");
    }

    //printf("Escreveu %s no disco\n", buffer);

    close(fd);
    free(buffer);
}

Cache *remove_file (Cache *cache, int pos) {
    if (cache->ocupados[pos] == EM_CACHE ) {
        free_metaDados(cache->docs[pos%cache->capacity]);
        cache->ocupados[pos] = LIVRE;
        push(cache->stack_to_cache, pos);
    } else if (cache->ocupados[pos] == EM_DISCO) {
        cache->ocupados[pos] = LIVRE;
        push(cache->stack_to_disc, pos);
        // nao ha necessidade de fazer nada com o disco 
        // porque o array ocupados é o que dita se posso escrever ou nao
    }
    return cache;
}

char *consult_file (Cache *cache, int pos) {
    if (cache->ocupados[pos] == EM_CACHE) {
        //printf("Consultado da cache na posição %d", pos);
    } else if (cache->ocupados[pos] == EM_DISCO) {
        //printf("Consultado do disco\n");
        if (cache->ocupados[pos%cache->capacity] == LIVRE) {
            //printf("Estava livre, então vou adicionar à cache\n");
            add_to_Cache(cache, desserializa_MetaDados(pos), pos);
        } else {
            add_to_Disk(cache, cache->docs[pos%cache->capacity]);
            add_to_Cache(cache, desserializa_MetaDados(pos), pos);
            cache->size--; // Decrementa o tamanho da cache, pois apenas é feita uma troca de documentos que já existem, logo o tamanho não muda
        }
    }
    return MD_toString(cache->docs[pos%cache->capacity], pos);
}

void free_Cache(Cache *docs) {
    if (docs == NULL) {
        return;
    }
    for (int i = 0; i < docs->capacity; i++) {
        if (docs->docs[i] != NULL) {
            free_metaDados(docs->docs[i]);
        }
    }
    free(docs->docs);
    free(docs->ocupados);
    free(docs);
}
 
void redimensiona_ocupados(Cache *docs) {
    //printf("Redimensionando ocupados...\n");

    int old_size = docs->capacity * docs->redimensionamentos;
    
    docs->redimensionamentos *= 2;

    int new_size = docs->capacity * docs->redimensionamentos;
    //printf("Old size: %d, New size: %d\n", old_size, new_size);

    char *new_ocupados = realloc(docs->ocupados, new_size * sizeof(char));
    if (new_ocupados == NULL) {
        perror("Realloc redimensiona_ocupados");
        exit(EXIT_FAILURE);
    }

    docs->ocupados = new_ocupados;
    for (int i = old_size; i < new_size; i++) {
        docs->ocupados[i] = LIVRE;
    }
 }
 
void redimensionar_auxiliares(Cache *docs) {
    redimensiona_ocupados(docs);
    increase_capacity(docs->stack_to_disc);
}
 
int documento_existe(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->size) {
        return -1;
    }
    else if (docs->ocupados[pos] == LIVRE) {
        return -2;
    } else if (docs->ocupados[pos] == EM_CACHE || docs->ocupados[pos] == EM_DISCO) {
        return 1;
    }
    return 0;
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
    int offset = pos * 520; // Supondo que cada documento ocupa 520 bytes
    lseek(fd, offset, SEEK_SET);
    read(fd, data, 520);
    close(fd);
    MetaDados *new = criar_metaDados(from_disk_format(data));
    return new;
}

char get_docs_estado(Cache *docs, int pos) {
    if (docs == NULL) {
        return '0';
    }
    return docs->ocupados[pos];
}

MetaDados *get_documento_cache(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->size) {
        return NULL;
    }
    if (docs->ocupados[pos] == EM_CACHE) {
        return docs->docs[pos % docs->capacity];
    } else {
        return NULL;
    }
}

MetaDados *get_anywhere_documento(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->size) {
        return NULL;
    }
    if (docs->ocupados[pos] == EM_CACHE) {
        return docs->docs[pos % docs->capacity];
    } else {
        char *data = malloc(520);
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
        int offset = pos * 520; // Supondo que cada documento ocupa 520 bytes
        lseek(fd, offset, SEEK_SET);
        read(fd, data, 520);
        close(fd);
        MetaDados *new = criar_metaDados(from_disk_format(data));
        if (new == NULL) {
            return NULL;
        }
        return new;
    }
}

void print_Cache (Cache *docs) {
    printf("Entrou na print_Cache\n");
    if (docs == NULL) {
        write(1, "Cache is NULL\n", 15);
        return;
    }
    write(1, "[Cache]\n\n", 10);
    for (int i = 0; i < docs->redimensionamentos * docs->capacity ; i++) {
        if (docs->ocupados[i] == EM_CACHE) {
            print_metaDados(docs->docs[i%docs->capacity]);
            write(1, "\n", 1);
        }
    }
    printf("Saiu da print_Cache\n");
}

int get_Max_docs (Cache *docs) {
    return docs->size;
}

void all_Cache_to_Disc (Cache *docs) {
    //printf("Adicionando todos os documentos à disco...\n");
    for (int i = 0; i < docs->redimensionamentos * docs->capacity ; i++) {
        //printf("Ocupados[%d]: %c\n", i, docs->ocupados[i]);
        if (docs->ocupados[i] == EM_CACHE) {
            //printf("Adicionando documento %d à disco...\n", i);
            add_to_Disk(docs, docs->docs[i%docs->capacity]);
            docs->ocupados[i] = EM_CACHE; // apesar de já estar na disco, o estado é cache
        }
    }
}

int get_cache_flag(Cache *docs) {
    return docs->dinamica;
}

void recupera_backup(Cache *cache){
    int fd = open(SERVER_STORAGE, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    char *data = malloc(520);
    if (data == NULL) {
        perror("Malloc disco_to_cache");
        return;
    }

    size_t n;
    while((n = read(fd, data, 520)) > 0){
        if (cache->dinamica == 0){//Cache estatica 
            if (cache->size >= cache->redimensionamentos*cache->capacity ) redimensionar_auxiliares(cache);

            add_to_Cache(cache, criar_metaDados(from_disk_format(data)), cache->size);
            cache->next_to_disc++;

        } else { //Cache dinamica
            if (cache->size < cache->capacity) {
                int i = cache->size;
                add_to_Cache(cache, criar_metaDados(from_disk_format(data)), i);
                cache->next_to_disc++;
        
            } else {
                // Se não houver espaço, aumentar o tamanho do array
                int new_max_docs = cache->capacity * 2;
                MetaDados **new_docs = realloc(cache->docs, new_max_docs * sizeof(MetaDados *));
                if (new_docs == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
                char *new_ocupados = realloc(cache->ocupados, new_max_docs * sizeof(char));
                if (new_ocupados == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
                cache->ocupados = new_ocupados;
                for (int i = cache->capacity; i < new_max_docs; i++) {
                    cache->ocupados[i] = LIVRE;
                }
        
                cache->docs = new_docs;
                cache->capacity = new_max_docs;
        
                if (cache->ocupados == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
        
                // Encontrar índice livre
                int i = 0;
                while (cache->ocupados[i] != LIVRE) i++;
                char* buffer = from_disk_format(data);
        
                add_to_Cache(cache, criar_metaDados(buffer), i);
                free(buffer);
                cache->next_to_disc++;
        
            }
        }
    }
    close(fd);
    free(data);
    
}
