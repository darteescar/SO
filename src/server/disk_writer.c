#include "server/disk_writer.h"

void write_to_disk() {

     if (mkfifo(DISK_WRITER_FIFO, 0666) == -1) {
          perror("MKFIFO disk_writer_fifo no write to disk"); 
          return;
     }

     int disk_fifo = open(DISK_WRITER_FIFO, O_RDWR);
     if (disk_fifo == -1) {
         perror("Open cache_fifo (leitura)");
         return;
     }

     int server_storage = open(SERVER_STORAGE, O_WRONLY | O_CREAT, 0644);
     if (server_storage == -1) {
          perror("open");
          return;
     }

     MetaDados *data = init_MD();

     while (1) {

          ssize_t bytes_read = read(disk_fifo, data, get_MD_size(data));

          if (bytes_read > 0) {

               int pos = get_MD_pos_in_disk(data);

               if (pos == -2) {
                    break; // Terminar o servidor
               }

               int offset = pos * 520;
               if (lseek(server_storage, offset, SEEK_SET) == -1) {
                    perror("lseek");
                    close(server_storage);
                    return;
               }

               char *buffer = to_disk_format_MD(data);
               if (write(server_storage, buffer, strlen(buffer)) == -1) {
                    perror("write");
               }

               free(buffer);
          } else {
               perror("read");
          }
     }
     
     free_MD(data);
     close(server_storage);
     close(disk_fifo);
     unlink(DISK_WRITER_FIFO);
}