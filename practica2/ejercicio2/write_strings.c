#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define BUF_SIZE 128

int main(int argc, char* argv[])
{
	if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_file> [text1 text2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

	FILE* stream = NULL;

	//abrir el fichero donde vamos a escribir
	stream = fopen(argv[1], "w");

	//si da error la llamada a fopen()
	if (stream == NULL)
	{
		err(3, "fopen() failed!\n");
	}

	//empezamos despues del 2o argumento (nombre del fichero donde escribir)
	int i = 2;

	//mientras no hayamos leido los demas elementos del array argv[]
	while (i < argc)
	{
		//tamanio del string que voy a escribir en el fichero (excluye '\0')
		unsigned int length = strlen(argv[i]);

		//puntero a char que apunta a la cadena de caracteres
		char* buf = argv[i];

		//nos aseguramos de guardar el caracter nulo
		buf[length] = '\0';
		
		//escribimos en fichero 
		size_t value = fwrite(argv[i], sizeof(char), length+1, stream);

		//si no hemos logrado escribir algun byte
		if (value == 0)
		{
			err(EXIT_FAILURE, "fwrite() could not write properly\n");
		}		

		//si logramos escribir algun byte
		else
			printf("Argument %d (%s) written succesfully!\n", i, argv[i]);

		++i;
	}	

	//cierre de fichero
	fclose(stream);

	printf("File %s closed succesfully!\n", argv[1]);

	return 0;
}
