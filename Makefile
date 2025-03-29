CC = gcc
CFLAGS = -Wall -g -Iinclude
LDFLAGS =

all: folders dserver dclient

dserver: bin/dserver

dclient: bin/dclient

folders:
	@mkdir -p src include obj obj/server obj/client bin tmp

# Lista todos os arquivos .c dentro de src/client/
SRC_CLIENT = $(wildcard src/client/*.c)

# Lista todos os arquivos .c dentro de src/server/
SRC_SERVER = $(wildcard src/server/*.c)

# Converte a lista de arquivos .c em .o na pasta obj/client/
OBJ_CLIENT = $(patsubst src/client/%.c, obj/client/%.o, $(SRC_CLIENT))

# Converte a lista de arquivos .c em .o na pasta obj/client/
OBJ_SERVER = $(patsubst src/server/%.c, obj/server/%.o, $(SRC_SERVER))

bin/dserver: $(OBJ_SERVER) obj/utils.o
	$(CC) $(LDFLAGS) $^ -o $@

bin/dclient: $(OBJ_CLIENT) obj/utils.o
	$(CC) $(LDFLAGS) $^ -o $@

obj/server/%.o: src/server/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/client/%.o: src/client/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/utils.o: src/utils.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f  obj/server/* obj/client/* obj/utils.o tmp/* bin/*
