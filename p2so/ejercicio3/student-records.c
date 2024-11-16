#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"
#include <err.h>

 extern char *optarg;

/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

char *loadstr(FILE *file);

int print_text_file(char *path)
{
	FILE* file = fopen(path, "r");	//apertura del fichero en modo lectura

	if (file == NULL)
	{
		perror("fopen()");
		fclose(file);

		return EXIT_FAILURE;
	}	

    int nEstudiantes;
    fscanf(file, "%d\n", &nEstudiantes); // leer numero de estudiantes

	for (int i = 0; i < nEstudiantes; i++)
	{
		student_t student; 
        char line[MAXLEN_LINE_FILE];

		fgets(line, sizeof(line), file);

		char *token;
		char *line_p = line;
		int count = 0;

		while ((token = strsep(&line_p, ":")) != NULL)
		{
			switch (count)
			{
				case STUDENT_ID_IDX:
					student.student_id = atoi(token);
					break;

				case NIF_IDX:
					strcpy(student.NIF, token);
					break;

				case FIRST_NAME_IDX:
					student.first_name = strdup(token);
					break;

				case LAST_NAME_IDX:
					//borra el salto de linea del final
					int len = strlen(token);
					token[len-1] = '\0';
					student.last_name = strdup(token);
			
			default:
				break;
			}
			count++;
		}
		
		printf("[Entry #%d]\n", i);
		printf("\tstudent_id=%d\n", student.student_id);
		printf("\tNIF=%s\n", student.NIF);
		printf("\tfirst_name=%s\n", student.first_name);
		printf("\tlast_name=%s\n", student.last_name);

	}
      
	fclose(file);	//cierre del fichero

	return 0;
}

int print_binary_file(char *path)
{
	FILE *inFile = fopen(path, "rb");	//apertura del fichero binario en modo lectura -> 'b' may be a good idea if you do  I/O to a binary file and expect that your program may be ported to non-UNIX environments.)

	if(inFile == NULL)
	{
		perror("fopen() infile");
		fclose(inFile);

		return EXIT_FAILURE;
	}

	int nEstudiantes;

	fread(&nEstudiantes,sizeof(int),1, inFile);	//leer numero de estudiantes

	for (int i = 0; i < nEstudiantes; i++)
	{ 
		student_t student;

		fread(&student.student_id, sizeof(int), 1, inFile);	//leer id estudiante
		fread(student.NIF, sizeof(student.NIF), 1, inFile);
		student.first_name = loadstr(inFile);	//leer primer nombre
		student.last_name = loadstr(inFile);	//leer apellido

		printf("[Entry #%d]\n", i);
        printf("\tstudent_id=%d\n", student.student_id);
        printf("\tNIF=%s\n", student.NIF);
        printf("\tfirst_name=%s\n", student.first_name);
        printf("\tlast_name=%s\n", student.last_name);		
	}
	
	fclose(inFile);	//cierre del fichero

	return 0;
}

char *loadstr(FILE *file)
{
	char c;
	int tam_string = 1;	//contamos caracter nulo

	while (fread(&c, sizeof(char),1, file) == 1 && c != '\0') 	//mientras 
	{
		tam_string++;	//actualizar contador
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

int write_binary_file(char *input_file, char *output_file)
{
	FILE* inFile = fopen(input_file, "r");	//apertura del fichero en modo lectura

	if(inFile == NULL)
	{
		perror("fopen() infile");

		return EXIT_FAILURE;
	}

	FILE* outFile = fopen(output_file, "w"); //apertura del fichero en modo escritura

	if (outFile == NULL)
	{
		perror("fopen() outfile");
		fclose(outFile);

		return EXIT_FAILURE;
	}

	int nEstudiantes;
	fscanf(inFile, "%d\n", &nEstudiantes);	//leer numero de estudiantes

	fwrite(&nEstudiantes,sizeof(int), 1, outFile);	//escribir el numero de estudiantes en el fichero binario

	for (int i = 0; i < nEstudiantes; i++)
	{
		 student_t student;
        char line[MAXLEN_LINE_FILE];

		fgets(line, sizeof(line), inFile);

		char *token;
		char *line_p = line;
		int count = 0;

		while ((token = strsep(&line_p, ":")) != NULL)
		{
			switch (count)
			{
				case STUDENT_ID_IDX:
					student.student_id = atoi(token);
					break;
			
				case NIF_IDX:
					strcpy(student.NIF, token);
					break;

				case FIRST_NAME_IDX:
					student.first_name = strdup(token);
					break;

				case LAST_NAME_IDX:
				//borra el salto de linea
					int len = strlen(token);
					token[len-1] = '\0';
					student.last_name = strdup(token);
					break;
				default:
					break;
			}
			count++;
		}
			
		fwrite(&student.student_id, sizeof(int), 1, outFile);
		fwrite(student.NIF, strlen(student.NIF)+1, 1, outFile);
		fwrite(student.first_name, strlen(student.first_name)+1, 1, outFile);
		fwrite(student.last_name, strlen(student.last_name)+1,1, outFile);		
	}	

	fclose(inFile);
    fclose(outFile);

    printf("%d student records written successfully to binary file %s\n", nEstudiantes, output_file);

	return 0;
}

int main(int argc, char *argv[])
{
	int ret_code, opt;
	struct options options;

	/* Initialize default values for options */
	options.input_file = NULL;
	options.output_file = NULL;
	options.action = NONE_ACT;
	ret_code = 0;

	/* Parse command-line options (incomplete code!) */
	while ((opt = getopt(argc, argv, "hi:po:b")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -p | -i infile | -o <output_file> | -b ]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case 'i':
			options.input_file = optarg;
			break;
		
		case 'p':
			options.action = PRINT_TEXT_ACT;	//activar flag de escritura en pantalla
			break;

		case 'o':
			options.output_file = optarg;
			options.action = WRITE_BINARY_ACT;
			break;

		case 'b':
			options.action = PRINT_BINARY_ACT;
			break;

		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.input_file == NULL)
	{
		fprintf(stderr, "Must specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
		ret_code = EXIT_FAILURE;
		break;
	case PRINT_TEXT_ACT:
		/* Part A */
		ret_code = print_text_file(options.input_file);
		break;
	case WRITE_BINARY_ACT:
		/* Part B */
		ret_code = write_binary_file(options.input_file, options.output_file);
		break;
	case PRINT_BINARY_ACT:
		/* Part C */
		ret_code = print_binary_file(options.input_file);
		break;
	default:
		break;
	}
	exit(ret_code);
}