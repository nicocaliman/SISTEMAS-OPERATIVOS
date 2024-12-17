#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) 
{
	FILE* file=NULL;
	int c,ret;

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	while (fread(&c, sizeof(char), 1,file) != 0/*(c = getc(file)) != EOF*/) 
	{
		/* Print byte to stdout */
		//ret=putc((unsigned char) c, stdout);
		ret = fwrite(&c, sizeof(char), 1, stdout);

		if (ret == 0)
		{
			fclose(file);
			err(3,"fwrite() failed!!");
		}
	}

	fclose(file);
	return 0;
}

/*
	fread(1,2,3,4): lee 3 cantidad de datos, cada uno con un 2 tamano de bytes, desde el fichero al que apunta el argumento 4, almacenandolo en el lugar dado por 1

	fwrite(1,2,3,4): escribe 3 cantidad de datos, cada uno de tamanio 2 byte al stream al que apunta 4 obteniendolo desde 1
 
	El parametro 1 es siempre un buffer desde el que se va a almacenar algo tras leer de archivo, o desde el que se va a leer algo para almacenarlo en un fichero

*/
