#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>

#define TAM_BLOQUE 512

void copy(int fdo, int fdd)
{
	char *buffer[TAM_BLOQUE];

	ssize_t char_leidos;

	while ((char_leidos = read(fdo, buffer, TAM_BLOQUE)) > 0)
	{
		write(fdd, buffer, char_leidos);
	}	
}

void copy_regular(char *orig, char *dest)
{
	int fdo;

	if ((fdo = open(orig, O_RDONLY)) == -1)
	{
		fprintf(stderr, "error al abrir el fichero origen");
		exit(EXIT_FAILURE);	
	}

	int fdd;

	if ((fdd = open(dest, O_WRONLY | O_TRUNC | O_CREAT)) == -1)
	{
		fprintf(stderr, "error al abrir el fichero destino");
		exit(EXIT_FAILURE);
	}

	copy(fdo, fdd);	//copiar como en el ejercicio 1

	//cierre de los descriptores de fichero
	close(fdo);
	close(fdd);	
}

void copy_link(char *orig, char *dest)
{
	struct stat stat_orig;

	if(lstat(orig, &stat_orig) == -1)	//lamamos a lstat() para conocer el numero de bytes que ocupa el enlace simbolico
	{
		perror("lstat lnk");
		exit(EXIT_FAILURE);
	}

	char *ruta_apuntada = (char*) malloc((stat_orig.st_size+1) * sizeof(char));	//reservar memoria para hacer una copia de la ruta apuntada 
	//stat_rig.st_size+1 pq el campo devuelve el tamanio del pathname pero sin el caracter terminador '\0'

	readlink(orig, ruta_apuntada, stat_orig.st_size);
	ruta_apuntada[stat_orig.st_size] = '\0';	//agregar manualmente el caracter nulo

	printf("org link route: %s\n", ruta_apuntada);

	symlink(ruta_apuntada, dest);	//crear un nuevo enlace simbólico que apunte a esta ruta.
	
	free(ruta_apuntada);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <orig_file> <dst_file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	struct stat stat_org;

	if (lstat(argv[1], &stat_org) == -1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}

	if (S_ISREG(stat_org.st_mode))	//si el fichero es regular
	{
		copy_regular(argv[1], argv[2]);	//hacer una copia identica a la del ejercicio 1
	}
	
	else if (S_ISLNK(stat_org.st_mode))	//si el fichero origen es un enlace simbolico
	{
		copy_link(argv[1], argv[2]);	//crear un enlace simbolico que apunte al mismo fichero al que apunta el fichero origen
	}
	
	else	
	{
		perror("otro tipo");
		exit(EXIT_FAILURE);
	}

	return 0;
}

/*
mylinkcopy es un fichero regular sin permisos 

ssoo@ssoohost:~/p3so/ejercicio1$ sudo cat mylinkcopy 
SRC = $(wildcard *.c)
TARGETS = $(SRC:%.c=%)

CC = gcc
CFLAGS = -g
LDFLAGS = 
LIBS = 

all: $(TARGETS)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGETS): %: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean:
	-rm $(TARGETS) $(SRC:%.c=%.o)

el contenido de mylinkcopy es un makefile 	

lstat() devuelve en su segundo argumento (un struct tipo stat) la informacion del fichero:

 struct stat {
               dev_t     st_dev;          ID of device containing file 
               ino_t     st_ino;          Inode number 
               mode_t    st_mode;         File type and mode 
               nlink_t   st_nlink;        Number of hard links 
               uid_t     st_uid;          User ID of owner 
               gid_t     st_gid;          Group ID of owner 
               dev_t     st_rdev;         Device ID (if special file) 
               off_t     st_size;         Total size, in bytes 
               blksize_t st_blksize;      Block size for filesystem I/O 
               blkcnt_t  st_blocks;       Number of 512B blocks allocated 

		...
		}

*/