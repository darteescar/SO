#include "utils/Message.h"

struct message{
     char buffer[512];
     int argc;
     int pid;
 };

Message *init_message() {
     Message *msg = malloc(sizeof(struct message));
     if (msg == NULL) {
          perror("malloc");
          exit(EXIT_FAILURE);
     }
     msg->buffer[0] = '\0';
     msg->argc = 0;
     msg->pid = 0;
     return msg;
 }
 
 void create_message(Message *msg, char *argv[], int argc, int pid) {
     if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
     }
     msg->argc = argc - 1;
     msg->pid = pid;
     msg->buffer[0] = '\0'; // limpa buffer
 
     for (int i = 1; i < argc; i++) {
          strcat(msg->buffer, argv[i]);
          if (i < argc - 1) {  // só se não for o último
              strcat(msg->buffer, FIELD_SEP);
          }
     }
}
 
size_t get_message_size(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     return sizeof(struct message);
}

void free_message(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          return;
     }
     free(msg);
 }

void print_message(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     printf("[Message]\n");
     printf("Buffer: %s\n", msg->buffer);
     printf("Argc: %d\n", msg->argc);
     printf("PID: %d\n", msg->pid);
 }
 
char get_message_command(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     return msg->buffer[1];
}
 
 int get_message_argc(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     return msg->argc;
 }
 
 char* get_message_buffer(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     return strdup(msg->buffer);
 }
 
int get_message_pid(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     return msg->pid;
 }

int get_key_msg(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     char *buffer = get_message_buffer(msg);
     buffer+=3; // Skip prefixo
     
     return atoi(buffer);
} 

char *get_keyword_msg(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     char *buffer = get_message_buffer(msg);
     buffer+=3;
     
     char *token = strsep(&buffer, FIELD_SEP);
     token = strsep(&buffer, FIELD_SEP);
     return token;
}

char *get_keyword_msg_s(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     char *buffer = get_message_buffer(msg);
     buffer+=3;

     char *token = strsep(&buffer, FIELD_SEP);
     return token;
}

int get_nProcessos_msg_s(Message *msg) {
     if (msg == NULL) {
         perror("Message is NULL");
         exit(EXIT_FAILURE);
     }
 
     char *buffer = get_message_buffer(msg);
     if (buffer == NULL) {
         perror("strdup");
         exit(EXIT_FAILURE);
     }
 
     char *ptr = buffer;
     char *token;
 
     token = strsep(&ptr, FIELD_SEP); // comando
     token = strsep(&ptr, FIELD_SEP); // keyword
     token = strsep(&ptr, FIELD_SEP); // n_procs

     if (token == NULL) {
          free(buffer);
     return 1; // ou valor por omissão
     }

     int res = atoi(token);
 
     free(buffer);
     return res;
 }

char *to_disk_format_message(struct message *msg) {
     if (msg == NULL) {
         return NULL;
     }
 
     // Aloca espaço para a string que conterá os dados serializados
     char *str = malloc(520);
     if (str == NULL) {
         perror("malloc");
         exit(EXIT_FAILURE);
     }
 
     // Serializa o buffer, argc e pid
     snprintf(str, 520, "%s|%d|%d", msg->buffer, msg->argc, msg->pid);
 
     return str;
 }

Message *from_disk_format_message(char *str) {
     if (str == NULL) {
         return NULL;
     }
 
     // Cria um novo Message
     Message *msg = init_message();
 
     // Separa os campos da string
     char *buffer = strsep(&str, "|");
     char *argc_str = strsep(&str, "|");
     char *pid_str = strsep(&str, "|");
 
     // Copia os dados para o Message
     strcpy(msg->buffer, buffer);
     msg->argc = atoi(argc_str);
     msg->pid = atoi(pid_str);
 
     return msg;
 }

void write_message(Message *msg, int fd) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     write (fd , msg , sizeof(Message));
}

Message *read_message(int fd) {
     Message *msg = init_message();
     if (msg == NULL) {
          perror("malloc");
          exit(EXIT_FAILURE);
     }
     read(fd, msg, sizeof(Message));
     return msg;
 }