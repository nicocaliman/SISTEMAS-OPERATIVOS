#include <stdio.h>
#include <stdlib.h>
#include <err.h>

// int main(int argc, char* argv[]) {
// 	FILE* file=NULL;
// 	int c,ret;

// 	if (argc!=2) {
// 		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
// 		exit(1);
// 	}

// 	/* Open file */
// 	if ((file = fopen(argv[1], "r")) == NULL)
// 		err(2,"The input file %s could not be opened",argv[1]);

// 	/* Read file byte by byte */
// 	while ((c = getc(file)) != EOF) {
// 		/* Print byte to stdout */
// 		ret=putc((unsigned char) c, stdout);

// 		if (ret==EOF){
// 			fclose(file);
// 			err(3,"putc() failed!!");
// 		}
// 	}

// 	fclose(file);
// 	return 0;
// }

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	unsigned char c;
	size_t num_items_read, num_items_written;

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	while ((num_items_read = fread(&c, sizeof(c),1,file)) != 0) {
		/* Print byte to stdout */
 		num_items_written = fwrite(&c , sizeof(c), 1, stdout);

		if (num_items_written == 0)	
		{
			fclose(file);
			err(3,"fwrite() failed!!");
		}		
 	}

	fclose(file);
	return 0;
}

/*
El codigo abre el fichero que se ha pasado por la terminal como segundo elemento del vector argv[]
Se abre dicho fichero en modo solo lectura (r). Ahora mientras no se llegue al final del fichero (lectura hasta el ultimo char)

Se almacena el elemento leido con getc() en la variable int al que luego se le hace un cast a (unsigned char).

Posteriormente, con putc() se escribe por terminal dicho caracter leido almacenado previamente en c.
*/