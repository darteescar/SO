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
 
     msg->argc = argc - 2;
     msg->pid = pid;
     msg->buffer[0] = '\0'; // limpa buffer
 
     for (int i = 1; i < argc; i++) {
         strcat(msg->buffer, argv[i]);
         if (i < argc - 1) {
             strcat(msg->buffer, FIELD_SEP); // separador seguro
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
 
 void print_message(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
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
     return msg->buffer;
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
/*
char *get_message_argv(Message *msg, int x) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     char *buffer = get_message_buffer(msg);
     buffer+=3; // Skip prefixo
     
     char *token;
     int i = 0;
     while ((token = strsep(&buffer, " ")) != NULL) {
          if (i == x) {
               return strdup(token);
          }
          i++;
     }
     return NULL;
}
*/
char *get_keyword_msg(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     char *buffer = get_message_buffer(msg);
     buffer+=3;

     char *token = strsep(&buffer, "+");
     token = strsep(&buffer, "+");
     return token;
}

char *get_keyword_msg_s(Message *msg) {
     if (msg == NULL) {
          perror("Message is NULL");
          exit(EXIT_FAILURE);
     }
     char *buffer = get_message_buffer(msg);
     buffer+=3;

     char *token = strsep(&buffer, "+");
     return token;
}