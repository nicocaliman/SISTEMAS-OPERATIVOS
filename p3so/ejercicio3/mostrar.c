#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


extern char *optarg;

extern int optind;

typedef enum mode
{
	FIRST,
	LAST

}tMode;

int main(int argc, char *argv[])
{
	int opt;
	int nBytes = 0;	//si -n no aparece N tomara el valor de 0

	tMode mode = FIRST;

	while((opt = getopt(argc, argv, "n:e")) != -1)
	{
		switch (opt)
		{
		case 'n':
			nBytes = atoi(optarg);
			break;
		
		case 'e':
			mode = LAST;	//leer ultimos N bytes del fichero 
			break;
			
		default:
			fprintf(stderr, "Usage: %s [-n N] [-e]\n", argv[0]);
            exit(EXIT_FAILURE);
		}
	}

	if (optind < argc - 1)
	{
		fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
	}

	int fdo;

	if ((fdo = open(argv[optind], O_RDONLY)) == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	

	if (mode == FIRST)
	{
		lseek(fdo, nBytes, SEEK_SET);	//si -e no aparece, leemos los primeros N bytes
	}

	else	
	{
		lseek(fdo, -nBytes, SEEK_END);	//si el argumento -e aparece, leemos los ultimos N bytes
	}
	
	ssize_t char_leidos;
	char c;

	while ((char_leidos = read(fdo, &c, 1)) > 0)	//leer byte a byte hasta EOF
	{
		write(1, &c, char_leidos);	//escribir byte a byte 
	}

	if (char_leidos == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
		
	close(fdo);	//cierre del descriptor de fichero

	return 0;
}
