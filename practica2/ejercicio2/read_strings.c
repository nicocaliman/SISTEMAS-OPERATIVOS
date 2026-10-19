#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char *loadstr(FILE *file)
{
	/* To be completed */
	char letra;
	int offset = 0;	//OJO CON PONER UNSIGNED INT PARA EL OFFSET Y LUEGO EN FSEEK PONER -OFFSET -> SEGMENTATION FAULT AL CONVERTIR DE UNSIGNED A SIGNED (SALE DEL RANGO)

	//mientras podamos leer y no sea el caracter nulo
	while(fread(&letra, sizeof(char), 1, file) > 0){

		//actualizamos la longitud de la cadena
		offset++;	

		if (letra == '\0')
		{
			break;
		}		
	}

	//reseteamos el puntero de posicion
	int value = fseek(file, -offset, SEEK_CUR);

	//para comprobar offset actual
	//printf("Current offset: %ld\n", ftell(file));

	//error en fseek
	if (value != 0)
	{
		err(EXIT_FAILURE, "fseek() failed!");
	}	

	//reservamos memoria dinamica 
	char* buffer = (char*) malloc(offset);

	//leer cadena entera inclusive caracter nulo
	//DEJA EL PUNTERO DE POSICION DEL FICHERO +1 POSICION DESPUES DEL OFFSET
	//EN ESTE CASO, SERIA DESPUES DEL CARACTER NULO
	size_t bytes_leidos = fread(buffer, sizeof(char), offset, file);

	// para comprobar offset actual
	// printf("Current offset: %ld\n", ftell(file));

	//si hemos leido minimo un byte
	if (bytes_leidos > 0)
	{
		return buffer;
	}
	//sino
	else
	{
		free(buffer);
		return NULL;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s <output_file> [text1 text2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
	}
	
	FILE* stream = NULL;

	//apertura fichero
	stream = fopen(argv[1], "r");

	//error de apertura
	if (stream == NULL)
	{
		err(EXIT_FAILURE, "fopen() failed!\n");
	}
	
	//lectura de cadena 
	char* ptr;
	char hop = '\n';

	//mientras no lleguemos al EOF (indicado por fseek())
	while((ptr = loadstr(stream)) != NULL){
		fwrite(ptr, sizeof(char), strlen(ptr), stdout);
		fwrite(&hop, sizeof(char), 1, stdout);

		//liberar memoria reservada
		free(ptr);			
	}

	//cierre fichero
	fclose(stream);

	return 0;
}
