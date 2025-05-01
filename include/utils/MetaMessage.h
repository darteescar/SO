#ifndef METAMESSAGE_H
#define METAMESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "Metadados.h"
#include "Message.h"

typedef struct metaMessage MetaMessage;

MetaMessage *init_MT();

void set_MT_message (MetaMessage *metaMessage, Message *msg);

void set_MT_meta_dados (MetaMessage *metaMessage, MetaDados *meta_dados);

int get_meta_message_size (MetaMessage *metaMessage);

MetaDados *get_MT_meta_dados (MetaMessage *metaMessage);

char get_MT_comando (MetaMessage *metaMessage);

int get_MT_key_msg (MetaMessage *metaMessage);

char *get_MT_keyword_msg (MetaMessage *metaMessage);

char *get_MT_keyword_msg_s (MetaMessage *metaMessage);

int get_MT_nProcessos_msg_s (MetaMessage *metaMessage);

int get_MT_msg_pid (MetaMessage *metaMessage);

void print_MT (MetaMessage *metaMessage);

char *to_disk_format_MT (struct metaMessage *metaMessage);

MetaMessage *from_disk_format_MT (const char *str);

void write_MT (int fd, MetaMessage *metaMessage);

MetaMessage *read_MT (int fd);

#endif