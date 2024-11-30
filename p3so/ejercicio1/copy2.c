#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define TAM_BLOQUE 512

void copy(int fdo, int fdd)
{
	char *buffer[TAM_BLOQUE];

	ssize_t chars_leidos;

	while(( chars_leidos = read(fdo, buffer, TAM_BLOQUE)) > 0)
	{
		write(fdd, buffer, chars_leidos);
	}
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <orig_file> <dst_file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int fdo;

	if((fdo = open(argv[1], O_RDONLY)) == -1)
	{
		fprintf(stderr, "error al abrir el fichero origen");
		exit(EXIT_FAILURE);
	}

	int fdd;

	if ((fdd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC)) == -1)
	{
		fprintf(stderr, "error al abrir el fichero destino");
		exit(EXIT_FAILURE);
	}
	
	copy(fdo, fdd);	//copiar fichero origen en fichero destino

	//cierre de descriptores de fichero
	close(fdo);
	close(fdd);

	return 0;
}

/*
	O_CREAT: si el nombre del fichero no existe, crea un fichero regular con ese nombre

	O_TRUNC: si el fichero existe y es un fichero regular y el modo de acceso admite ESCRIBIR(O_WRONLY), se trunca a longitud 0

	O_WRONLY: abrir el fichero en modo escritura

	O_RDONLY : abrir el fichero en modo lectura

	read() devuelve el numero de chars leidos, 0 si llega al final del fichero
*/