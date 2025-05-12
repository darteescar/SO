/**
 * @file disk_writer.h
 * @brief Header file for disk_writer.c
 * @author Group 37 de SO
 */
#ifndef DISK_WRITER_H
#define DISK_WRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "utils/Metadados.h"
#include "utils/Cache.h"

#define DISK_WRITER_FIFO "tmp/disk_writer_fifo"

/**
 * @brief Função que guarda os MetaDados em disco lendo-as do fifo DISK_WRITER_FIFO.
 */
void write_to_disk();

#endif