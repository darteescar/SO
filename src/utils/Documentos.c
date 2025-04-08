#include "utils/Documentos.h"

struct documentos{
    int n_docs;
    int max_docs;
    MetaDados **docs;
};

Documentos *create_documentos(int max_docs) {
    Documentos *docs = malloc(sizeof(struct documentos));
    if (docs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    docs->n_docs = 0;
    docs->max_docs = max_docs;
    docs->docs = malloc(max_docs * sizeof(MetaDados*));
    if (docs->docs == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return docs;
}

void add_documento(Documentos *docs, MetaDados *doc) {
    if (docs->n_docs >= docs->max_docs) {
        increase_size_documentos(docs);
    }
    docs->docs[docs->n_docs] = doc;
    docs->n_docs++;
}

void increase_size_documentos(Documentos *docs) {
    docs->max_docs *= 2;
    docs->docs = realloc(docs->docs, docs->max_docs * sizeof(MetaDados*));
    if (docs->docs == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
}

MetaDados *get_documento(Documentos *docs, int index) {
    if (index < 0 || index >= docs->n_docs) {
        return NULL;
    }
    return docs->docs[index];
}

void print_documentos(Documentos *docs) {
    for (int i = 0; i < docs->n_docs; i++) {
        print_metaDados(get_documento(docs, i));
    }
}