#ifndef AVERAGE_TIME_CLIENTS_H
#define AVERAGE_TIME_CLIENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 4096
#define LINE_MAX 128

int average_time_clients();

#endif