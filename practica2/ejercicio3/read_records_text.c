#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define LABEL_MAX_LEN 15

typedef struct 
{
    int id;
    double value;
    char label[LABEL_MAX_LEN+1];
}SimpleRecord;


int main(int argc, char* argv[]){

    FILE* stream = NULL;

    //apertura del fichero
    stream = fopen(argv[1], "r");

    //error de apertura
    if (stream == NULL)
    {
        err(EXIT_FAILURE, "fopen() failed!");
    }

    SimpleRecord sr;

    //si los valores escaneados son 3 (id, label, value), fscanf() DEVUELVE EL NUMERO DE ITEMS MATCHEADOS 
    //%lf para variables double
    while (fscanf(stream, "%s %lf %d", sr.label, &sr.value, &sr.id) == 3)
    {
        fprintf(stdout, "ID: %d, Valor: %.2f, Etiqueta: '%s'\n", sr.id, sr.value, sr.label);
    }
    
    //cierre del fichero
    fclose(stream);

    return 0;
}