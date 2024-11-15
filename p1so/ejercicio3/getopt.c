#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include "defs.h"

char * progname;

/**
 * @brief  Print usage of the tool
 **/
void usage() {
	printf("Usage: %s [ options ] title \n",progname);
	printf("\n\toptions:\n");
	printf("\t\t-h: display this help message\n");
	printf("\t\t-e: print even numbers instead of odd  (default)\n");
	printf("\t\t-l lenght: lenght of the sequence to be printed\n");
	printf("\t\ttitle: name of the sequence to be printed\n");
}

/**
 * @brief  Prints a sequence of odd or even numbers in stdout
 *
 * @param lenght
 * @param type
 * @param title
 **/
void display_numbers ( int lenght, parity_t type, char * title) {
	
	int i,n;
	int first = (type == ODD )? 1 : 2;
	
	printf("Title: %s\n",title);
	for (i=0,n=first; i<lenght; i++,n+=2) {
		printf("%d ",n);
	}
	printf("\n");
}

extern char *optarg;

extern int optind;

int main(int argc, char *argv[])
{
	int  opt;
	struct options options;

	progname = argv[0];

	/* Initialize default values for options */
	options.par_mode = ODD;
	options.lenght= 10;
	options.title = NULL;

	/* Parse command-line options */
	while((opt = getopt(argc, argv, "hel:")) != -1) {
		switch(opt) {
		case 'h':
			usage();
			exit(0);
		case 'e':
			options.par_mode = EVEN;	//print even numbers
			break;
		case 'l':
			options.lenght = strtol(optarg, NULL, 10);	//strtol()
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	/* There should be one extra argument (the title of the sequence) */
	if (optind >= argc) {
		fprintf(stderr, "Invalid title\n");
		usage();
		exit(EXIT_FAILURE);
	}
	
	/* Fill options.title with the corresponding element of argv */
		options.title = argv[optind];

    /* Call display_numbers */
	display_numbers (options.lenght, options.par_mode, options.title);
	return 0;
}

/*
la funcion getopt() se utiliza para parsear opciones en una linea de comandos(./getopt -l -e -m...).

El 3er argumento de getopt() indica cuales son las opciones que el programa acepta

Los parametros se parsean de izquierda a derecha, cada parametro se parsea como una opcion corta, larga.

En este caso tenemos opciones cortas(-).

La variable optind es el indice del siguiente elemento a ser procesado en argv[].
El sistema inicia este valor a 1. Si se encuentra otro caracter 'opcion' este valor se actualiza.

Si no hay mas opciones getopt() devuelve -1.

Si la opcion requiere de un argumento se almacena en optarg

strtol(1, 2, 3) -> lo que hace esta funcion es convertir la parte initial de un string en un valor entero en base 3er parametro

en este caso lo que hacemos es pasar como 1er parametro el valor de optarg que sera el numero(longitud), 
el segundo parametro es indiferente ya que nos sirve para devolver la direccion del primer caracter de la cadena que no se ha convertido
Es decir, nos va a ser muy util para verificar si la conversion se realizo correctamente o si hay caracteres que no son numeros.

El 3er parametro es la base en la que se representa el valor de optarg.
*/