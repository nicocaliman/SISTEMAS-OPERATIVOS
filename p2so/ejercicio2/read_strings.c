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
	char c;
	int tam_string = 1;	//contamos caracter nulo

	while (fread(&c, sizeof(char),1, file) == 1 && c != '\0') 	//mientras no encontremos el caracter nulo y la lectura sea exitosa
	{
		tam_string++;	//actualizar contador (no se va a contar '\0')
	}

	if (tam_string == 1)
	{
		return NULL;
	}

	fseek(file, -(sizeof(char)*tam_string), SEEK_CUR);	

	char *str = malloc(tam_string*sizeof(char));	//reservar memoria dinamica para la cadena leida

	if (str == NULL)
	{
		err(2, "malloc function");
		exit(1);
	}

	fread(str, sizeof(char)*tam_string, 1, file);	//leer la cadena completa	
	
	return str;	
}

int main(int argc, char *argv[])
{
	FILE *file = NULL;

	if (argc < 2) 
	{
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

    if((file = fopen(argv[1], "r")) == NULL)	//apertura del archivo en modo lectura
	{
		//si null		
		err(2,"The input file %s could not be opened",argv[1]);
		return EXIT_FAILURE;
	}

	char *str;
    while ((str = loadstr(file)) != NULL) 
	{
        printf("%s\n", str);
        free(str); // Liberar la memoria de la cadena
    }

	fclose(file);	//cierre del fichero
	return 0;
}
