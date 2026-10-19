#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <err.h>
#include "defs.h"

char * progname;

#define LABEL_MAX_LEN 15

typedef struct 
{
    int id;
    double value;
    char label[LABEL_MAX_LEN+1];
}SimpleRecord;

/**
 * @brief  Print usage of the tool
 **/
void usage() {
	printf("Usage: %s [-i t|b] [-o t|b] input_file output_file \n",progname);
	printf("\t\t-i: input file format (\"t\": text | \"b\": binary) [optional]\n");
	printf("\t\t-o: output file format (\"t\": text | \"b\": binary) [optional]\n");
	printf("\t\tinput_file: name of the file to read data\n");
    printf("\t\toutput_file: name of the file to store data\n");
}

/**
 * Lee un único registro del stream. 
 * Devuelve 1 si la lectura fue exitosa, o 0 si llegó al final del fichero/error.
 **/
int read_record(FILE *stream, SimpleRecord *sr, action_t action) {
    //text
    if (action == READ_TEXT_ACT) {
        return (fscanf(stream, "%15s %lf %d", sr->label, &sr->value, &sr->id) == 3);
    } 
    //binary
    else {
        return (fread(sr, sizeof(SimpleRecord), 1, stream) == 1);
    }
}

/**
 * Escribe un único registro en el stream.
 **/
void write_record(FILE *stream, const SimpleRecord *sr, action_t action) {
    //text
    if (action == WRITE_TEXT_ACT) {
        fprintf(stream, "%s %.2f %d\n", sr->label, sr->value, sr->id);
    } 
    //binary
    else {
        fwrite(sr, sizeof(SimpleRecord), 1, stream);
    }
}

int main(int argc, char *argv[])
{
	int  opt;
    struct options options;

    options.action_read = READ_TEXT_ACT;
    options.action_write = WRITE_TEXT_ACT;

	//nombre del primer argumento que se pasa a la terminal (./nombrePrograma)
	progname = argv[0];

	/* Parse command-line options */
	while((opt = getopt(argc, argv, "hi:o:")) != -1) {
		switch(opt) {
		case 'h':
			usage();
			exit(0);
		case 'i':
            if (strcmp(optarg, "t") == 0) {
                options.action_read = READ_TEXT_ACT;
            } else if (strcmp(optarg, "b") == 0) {
                options.action_read = READ_BINARY_ACT;
            } else {
                err(EXIT_FAILURE, "Input format incorrect! Must be 't' or 'b'.");
            }
			break;
		case 'o':
            if (strcmp(optarg, "t") == 0) {
                options.action_write = WRITE_TEXT_ACT;
            } else if (strcmp(optarg, "b") == 0) {
                options.action_write = WRITE_BINARY_ACT;
            } else {
                err(EXIT_FAILURE, "Output format incorrect! Must be 't' or 'b'.");
            }
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

    //pillamos el nombre del fichero de entrada y fichero de salida
    options.input_file = argv[optind];
    options.output_file = argv[optind + 1];

    // 1. Determinar y abrir el flujo de entrada
    FILE *in_stream = NULL;
    //si el input_file es "-" -> leemos de stdin
    if (strcmp(options.input_file, "-") == 0) {
        in_stream = stdin;
    } 
    //sino
    else {
        char *mode_in = (options.action_read == READ_BINARY_ACT) ? "rb" : "r";
        //abrir fichero de entrada
        in_stream = fopen(options.input_file, mode_in);
        
        //fallo de apertura
        if (in_stream == NULL) {
            err(EXIT_FAILURE, "Error abriendo fichero de entrada '%s'", options.input_file);
        }
    }

    // 2. Determinar y abrir el flujo de salida
    FILE *out_stream = NULL;

    //si el output_file es "-" -> escribimos en stdout (terminal)
    if (strcmp(options.output_file, "-") == 0) {
        out_stream = stdout;
    } 
    //sino
    else {
        char *mode_out = (options.action_write == WRITE_BINARY_ACT) ? "wb" : "w";

        //apertura de fichero
        out_stream = fopen(options.output_file, mode_out);

        //error de apertura
        if (out_stream == NULL) {
            if (in_stream != stdin) fclose(in_stream);
            err(EXIT_FAILURE, "Error abriendo fichero de salida '%s'", options.output_file);
        }
    }

    // 2. Bucle de conversión registro a registro
    SimpleRecord sr;

    //mientras se siga pudiendo leer un struct del fichero de entrada
    while (read_record(in_stream, &sr, options.action_read) == 1) {
        write_record(out_stream, &sr, options.action_write);
    }

   // 4. Cierre condicional (solo cerramos si se abrieron con fopen)
    if (in_stream != stdin) {
        fclose(in_stream);
    }
    if (out_stream != stdout) {
        fclose(out_stream);
    }

	return 0;
}