#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>

#define LABEL_MAX_LEN 15
#define TAM_ARR 3

typedef struct 
{
    int id;
    double value;
    char label[LABEL_MAX_LEN+1];
}SimpleRecord;


int main(int argc, char* argv[]){

    SimpleRecord array[TAM_ARR];
    
    //inicializar campos de struct 
    array[0].id = 1;
    strcpy(array[0].label, "Barcelona");    //copia dst <- org
    array[0].value = 3.10;
    
    array[1].id = 0;
    strcpy(array[1].label, "Madrid");       //copia dst <- org
    array[1].value = 19.77;

    
    array[2].id = 2;
    strcpy(array[2].label, "Valencia");     //copia dst <- org
    array[2].value = 7.42;

    FILE* stream = NULL;

    //apertura del fichero
    stream = fopen(argv[1], "w");

    //error de apertura
    if (stream == NULL)
    {
        err(EXIT_FAILURE, "fopen() failed!");
    }

    //recorrer array
    for (int i = 0; i < TAM_ARR; i++)
    {
        fprintf(stream, "%s %.2f %d\n", array[i].label, array[i].value, array[i].id);   
    }

    printf("La escritura ha finalizado correctamente\n");
        
    //cierre del fichero
    fclose(stream);

    return 0;
}