# SO (Sistemas Operativos) (Português)
Implementação de um Serviço de Indexação e Pesquisa de documentos texto guardados localmente num computador. Pode consultar o [enunciado] (Requisitos_SO.pdf) e o [relatório] (Relatório_SO.pdf) do projeto.

### Nota Final: ?? / 20 ⭐️

## Membros do grupo

* [darteescar](https://github.com/darteescar)
* [luis7788](https://github.com/luis7788)
* [tiagofigueiredo7](https://github.com/tiagofigueiredo7)

## Executável

Para compilar o servidor e o cliente, basta fazer:

```console
$ cd SO
$ make
```

### Programa principal

Para executar o servidor, basta fazer:

```console
$ ./dserver Dataset size
```   
Sendo `size` a capacidade do servidor.

Para executar o cliente, é possível a realização de várias operações, como por exemplo:

```console
$ ./dclient -a título autores ano path
$ ./dclient -c chave
$ ./dclient -d chave
$ ./dclient -s palavra-chave nr_processos
$ ./dclient -l chave palavra-chave
$ ./dclient -f 
$ ./dclient -b
```

O cliente tem várias opções, como podem ver acima. Para mais informações sobre o funcionamento do cliente, podem consultar o [relatório] (Relatório_SO.pdf) do projeto.

### Remover ficheiros
Para remover os executáveis e outros ficheiros, basta fazer:

```console
$ make clean
```
Note que o `make clean` não remove os ficheiros de dados, apenas os executáveis e outros ficheiros gerados durante a compilação e, por isso, o 'server_storage' não é removido.

# SO (Operating Systems) (English)
Implementation of a Service for Indexing and Searching text documents stored locally on a computer. You can consult the [statement] (Requisitos_SO.pdf) and the [report] (Relatório_SO.pdf) of the project.

### Final Grade: ?? / 20 ⭐️

## Group members

* [darteescar](https://github.com/darteescar)
* [luis7788](https://github.com/luis7788)
* [tiagofigueiredo7](https://github.com/tiagofigueiredo7)

## Executable
To compile the server and client, just do:

```console
$ cd SO
$ make
```
### Main program
To run the server, just do:

```console
$ ./dserver Dataset size
```   
Where `size` is the server's capacity.
To run the client, it is possible to perform several operations, such as:

```console
$ ./dclient -a title authors year path
$ ./dclient -c key
$ ./dclient -d key
$ ./dclient -s keyword nr_processes
$ ./dclient -l key keyword
$ ./dclient -f 
$ ./dclient -b
```
The client has several options, as you can see above. For more information on how the client works, you can check the [report] (Relatório_SO.pdf) of the project.
### Remove files
To remove the executables and other files, just do:

```console
$ make clean
```
Note that `make clean` does not remove the data files, only the executables and other files generated during compilation, so 'server_storage' is not removed.