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

Cache *create_Cache(int max_docs, int flag_dinamica) {
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
    docs->dinamica = flag_dinamica;
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
 
Cache *add_documento(Cache *cache, MetaDados *data, int *pos_onde_foi_add) {
    if (cache->dinamica == CACHE_ESTATICA) {
        return add_documento_Estaticamente(cache, data, pos_onde_foi_add);
    } else {
        return add_documento_Dinamicamente(cache, data, pos_onde_foi_add);
    }
}

Cache *add_documento_Estaticamente(Cache *cache, MetaDados *mt, int *pos_onde_foi_add){

    int pid = get_MD_pid(mt);

    // Se for um MetaDado restaurado (sem PID), adicionar diretamente na posição definida
    if (pid == -1) {
        int pos = get_MD_pos_in_disk(mt);

        if (pos < 0 || pos >= cache->capacity * cache->redimensionamentos) {
            fprintf(stderr, "Posição inválida no metadado restaurado: %d\n", pos);
            return cache;
        }

        int value_stack = existe_MapPos_stack(cache->stack_to_cache, pos, cache->capacity);

        if (value_stack != -1) {//Essa possição está livre na cache
            remove_pos_stack(cache->stack_to_cache, value_stack);
            add_to_Cache(cache, mt, pos);
            cache->size--;
            *pos_onde_foi_add = pos;
            return cache;
        }

        add_to_Disk(cache, cache->docs[pos%cache->capacity]);
        add_to_Cache(cache, mt, pos);
        cache->size--;
        *pos_onde_foi_add = pos;
        return cache;
    }

    if (cache->size < cache->capacity) {
        *pos_onde_foi_add = cache->size;
        add_to_Cache(cache, mt, cache->size);
    }
    else {
        if (cache->size >= cache->redimensionamentos*cache->capacity ) redimensionar_auxiliares(cache);

        if (is_empty(cache->stack_to_cache)){
            if (is_empty(cache->stack_to_disc)) {

                add_to_Disk(cache, cache->docs[cache->next_to_disc%cache->capacity]);
                free_MD(cache->docs[cache->next_to_disc%cache->capacity]);
                add_to_Cache(cache, mt, cache->next_to_disc);
                *pos_onde_foi_add = cache->next_to_disc;
                cache->next_to_disc++;

            } else {
                int pos = pop(cache->stack_to_disc);
                set_MD_disk_position(mt, pos);
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

Cache *add_documento_Dinamicamente(Cache *cache, MetaDados *mt, int *pos_onde_foi_add){
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

        add_to_Cache(cache, mt, i);
        *pos_onde_foi_add = i;
    }
    return cache;
}

void add_to_Cache(Cache *cache, MetaDados *data, int pos) {
    if (get_MD_pos_in_disk(data) == -1) {
        set_MD_disk_position(data, pos);
    }
    cache->docs[pos%cache->capacity] = data;
    cache->ocupados[pos] = EM_CACHE;
    cache->size++;
}

void add_to_Disk(Cache *cache, MetaDados *data) {
    int pos = get_MD_pos_in_disk(data);
    cache->ocupados[pos] = EM_DISCO;

    int fd = open(DISK_WRITER_FIFO, O_WRONLY);
    if (fd == -1) {
        perror("Open add_to_Disk");
        return;
    }
    if (write(fd, data, get_MD_size(data)) == -1) {
        perror("Write add_to_Disk");
        close(fd);
        return;
    }
    close(fd);
}

Cache *remove_file (Cache *cache, int pos) {
    if (cache->ocupados[pos] == EM_CACHE ) {
        cache->ocupados[pos] = LIVRE;
        free_MD(cache->docs[pos%cache->capacity]);
        if (cache->dinamica == CACHE_ESTATICA) push(cache->stack_to_cache, pos);//se for cache dinamica não se usa stack
    } else if (cache->ocupados[pos] == EM_DISCO) {
        cache->ocupados[pos] = LIVRE;
        push(cache->stack_to_disc, pos);
        // nao ha necessidade de fazer nada com o disco 
        // porque o array ocupados é o que dita se posso escrever ou nao
    }
    return cache;
}

void free_Cache(Cache *cache) {
    if (cache == NULL) {
        return;
    }

    destruir_stack(cache->stack_to_cache);
    destruir_stack(cache->stack_to_disc);

    free(cache->docs);

    free(cache);
}
 
void redimensiona_ocupados(Cache *docs) {
    int old_size = docs->capacity * docs->redimensionamentos;
    
    docs->redimensionamentos *= 2;

    int new_size = docs->capacity * docs->redimensionamentos;

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

MetaDados *get_anywhere_documento(Cache *docs, int pos) {
    if (docs == NULL || pos < 0 || pos >= docs->size) {
        return NULL;
    }
    if (docs->ocupados[pos] == EM_CACHE) {
        return docs->docs[pos % docs->capacity];
    } else {
        MetaDados *new = desserializa_MetaDados(pos);
        return new;
    }
}

void print_Cache (Cache *docs) {
    printf("[CACHE]\n");
    for (int i = 0; i < docs->capacity ; i++) {
        print_MD(docs->docs[i]);
    }

    printf("[FIM CACHE]\n");
    printf("\n");
}

int get_Max_docs (Cache *docs) {
    return docs->size;
}

void all_Cache_to_Disc (Cache *docs) {
    for (int i = 0; i < docs->redimensionamentos * docs->capacity ; i++) {
        if (docs->ocupados[i] == EM_CACHE) {
            add_to_Disk(docs, docs->docs[i%docs->capacity]);
            docs->ocupados[i] = EM_CACHE; // apesar de já estar na disco, o estado é cache
        }
    }
}

int get_cache_flag(Cache *docs) {
    return docs->dinamica;
}

char get_cache_estado(Cache *cache, int pos) {
    return cache->ocupados[pos];
}

Cache* reset_Cache(Cache *cache){
    int flag = get_cache_flag(cache);
    Cache* new = NULL;
    if (flag == 0){
        new = create_Cache(cache->capacity,0);
    } else {
        new = create_Cache(10,1);
    }
    free_Cache(cache);
    return new;
}

Cache* recupera_backup(Cache *cache, MetaDados *msg){
    char* file_path = get_MD_something(msg, 1);
    if (file_path == NULL) {
        perror("get_MD_something");
        return NULL;
    }

    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }

    char *data = malloc(520);
    if (data == NULL) {
        perror("Malloc disco_to_cache");
        return NULL;
    }

    cache = reset_Cache(cache);

    size_t n;
    while((n = read(fd, data, 520)) > 0){
        if (cache->dinamica == CACHE_ESTATICA){//Cache estatica 
            if (cache->size >= cache->redimensionamentos*cache->capacity ) redimensionar_auxiliares(cache);

            add_to_Cache(cache, criar_metaDados(from_disk_format_MD(data)), cache->size);
            add_to_Disk(cache, cache->docs[(cache->size-1)%cache->capacity]);
            cache->next_to_disc++;

        } else { //Cache dinamica
            if (cache->size < cache->capacity) {
                int i = cache->size;
                add_to_Cache(cache, criar_metaDados(from_disk_format_MD(data)), i);
        
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
                char* buffer = from_disk_format_MD(data);
        
                add_to_Cache(cache, criar_metaDados(buffer), i);
                free(buffer);
        
            }
        }
    }
    if(cache->dinamica==CACHE_ESTATICA) {
        cache->next_to_disc-=cache->capacity;
        for(int j=cache->size-cache->capacity; j<cache->size; j++){
            cache->ocupados[j] = EM_CACHE;
        }
    }
    close(fd);
    free(data);

    return cache; 
}