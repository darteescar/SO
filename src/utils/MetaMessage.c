#include "utils/MetaMessage.h"

struct metaMessage {
     MetaDados *meta_dados;
     Message *msg;
};

MetaMessage *init_MT() {
     MetaMessage *metaMessage = (MetaMessage *)malloc(sizeof(MetaMessage));
     if (metaMessage == NULL) {
          perror("Failed to allocate memory for MetaMessage");
          return NULL;
     }
     metaMessage->meta_dados = init_MD();
     metaMessage->msg = init_message();
     return metaMessage;
}

void set_MT_message (MetaMessage *metaMessage, Message *msg) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em set_MT_message");
          return;
     }
     metaMessage->msg = msg;
}

void set_MT_meta_dados (MetaMessage *metaMessage, MetaDados *meta_dados) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em set_MT_meta_dados");
          return;
     }
     metaMessage->meta_dados = meta_dados;
}

int get_meta_message_size (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_meta_message_size");
          return -1;
     }
     return get_message_size(metaMessage->msg) + get_MD_size(metaMessage->meta_dados); // por definir
}

MetaDados *get_MT_meta_dados (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_MT_meta_dados");
          return NULL;
     }
     return metaMessage->meta_dados;
}

char get_MT_comando (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_MT_comando");
          return -1;
     }
     return get_message_command(metaMessage->msg);
}

int get_MT_key_msg (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_MT_key_msg");
          return -1;
     }
     return get_key_msg(metaMessage->msg);
}

char *get_MT_keyword_msg (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_MT_key_msg");
          exit(EXIT_FAILURE);
     }
     return get_keyword_msg(metaMessage->msg);
}

char *get_MT_keyword_msg_s (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_MT_key_msg");
          exit(EXIT_FAILURE);
     }
     return get_keyword_msg_s(metaMessage->msg);
}

int get_MT_nProcessos_msg_s (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_nProcessos_msg_s");
          return -1;
     }
     return get_nProcessos_msg_s(metaMessage->msg);
}

int get_MT_msg_pid (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em get_MT_msg_pid");
          return -1;
     }
     return get_message_pid(metaMessage->msg);
}

void print_MT (MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em print_MT_message");
          return;
     }
     print_message(metaMessage->msg);
     print_MD(metaMessage->meta_dados);
}


// Função principal para serializar a estrutura metaMessage
char *to_disk_format_MT (struct metaMessage *metaMessage) {
    if (metaMessage == NULL) {
        perror("MetaMessage é NULL em to_disk_format");
        return NULL;
    }

    // Aloca um buffer para armazenar o resultado final
    char *str = malloc(1040); // Tamanho maior para acomodar as duas partes
    if (str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Serializa o meta_dados usando a função específica para metaDados
    char *metaDadosStr = to_disk_format_MD(metaMessage->meta_dados);
    if (metaDadosStr == NULL) {
        free(str);
        return NULL;
    }

    // Serializa a message usando a função específica para message
    char *messageStr = to_disk_format_message(metaMessage->msg);
    if (messageStr == NULL) {
        free(str);
        free(metaDadosStr);
        return NULL;
    }

    // Combina as duas partes
    snprintf(str, strlen(metaDadosStr) + strlen(messageStr) , "%s|%s", metaDadosStr, messageStr);

    // Libera as strings temporárias
    free(metaDadosStr);
    free(messageStr);

    return str;
}

// Função para desserializar metaMessage
MetaMessage* from_disk_format_MT (const char* str) {
     struct metaMessage* metaMessage = malloc(sizeof(struct metaMessage));
     if (metaMessage == NULL) {
         perror("malloc");
         return NULL;
     }
 
     // Divida a string usando o delimitador "|"
     char* str_copy = strdup(str);
     char* meta_dados_str = strtok(str_copy, "|");
     char* msg_str = strtok(NULL, "");
 
     // Desserializa metaDados
     metaMessage->meta_dados = criar_metaDados(from_disk_format_MD(meta_dados_str));
 
     // Desserializa message
     metaMessage->msg = from_disk_format_message(msg_str);
 
     free(str_copy);  // Libera a cópia da string original
 
     return metaMessage;
 }

void write_MT (int fd, MetaMessage *metaMessage) {
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em write_MT");
          return;
     }
     write_MD(metaMessage->meta_dados, fd );
     write_message( metaMessage->msg, fd);
}

MetaMessage *read_MT (int fd) {
     MetaMessage *metaMessage = init_MT();
     if (metaMessage == NULL) {
          perror("MetaMessage é NULL em read_MT");
          return NULL;
     }
     metaMessage->meta_dados = read_MD (fd);
     metaMessage->msg = read_message(fd);
     return metaMessage;
}