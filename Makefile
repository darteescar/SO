CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS =

all: folders dserver dclient

dserver: bin/dserver

dclient: bin/dclient

folders:
	@mkdir -p src include obj obj/server obj/client obj/utils bin tmp

# Lista todos os arquivos .c dentro de src/client/
SRC_CLIENT = $(wildcard src/client/*.c)

# Lista todos os arquivos .c dentro de src/server/
SRC_SERVER = $(wildcard src/server/*.c)

# Lista todos os arquivos .c dentro de src/utils/
SRC_UTILS = $(wildcard src/utils/*.c)

# Converte a lista de arquivos .c em .o na pasta obj/client/
OBJ_CLIENT = $(patsubst src/client/%.c, obj/client/%.o, $(SRC_CLIENT))

# Converte a lista de arquivos .c em .o na pasta obj/server/
OBJ_SERVER = $(patsubst src/server/%.c, obj/server/%.o, $(SRC_SERVER))

# Converte a lista de arquivos .c em .o na pasta obj/utils/
OBJ_UTILS = $(patsubst src/utils/%.c, obj/utils/%.o, $(SRC_UTILS))

bin/dserver: $(OBJ_SERVER) $(OBJ_UTILS)
	$(CC) $(LDFLAGS) $^ -o $@
	ln -sf bin/dserver dserver  

bin/dclient: $(OBJ_CLIENT) $(OBJ_UTILS)
	$(CC) $(LDFLAGS) $^ -o $@
	ln -sf bin/dclient dclient

obj/server/%.o: src/server/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/client/%.o: src/client/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/utils/%.o: src/utils/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f  obj/server/* obj/client/* obj/utils/* tmp/* bin/* dserver dclient