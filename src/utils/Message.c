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
     msg->argc = argc-2;
     msg->pid = pid;
     for (int i = 1; i < argc; i++) {
          strcat(msg->buffer, argv[i]);
          if (i < argc - 1) {
               strcat(msg->buffer, " "); // adiciona espaço entre os argumentos (podia ser um espaço ou outro delimitador)
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