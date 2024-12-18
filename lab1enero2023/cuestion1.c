#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXPROC 16
#define MAX_LINE_FILE 100

struct task
{
    int valid;  //entrada valida(1), no valida(0)
    int next[MAXPROC];  //array de booleanos para sucesores, next[i] = 1 si sale una arista hacia el nodo i
    int id; //nombre de la tarea, que corresponde a su posicion en el array de nodos
};

struct task tasks[MAXPROC]; //variable global

void print_graph(struct task tasks[], int n)
{
    int i,j;

    for (i = 0; i < n; i++)
    {
        if (!tasks[i].valid)
        {
            continue;
        }
        printf("%d: ", tasks[i].id);

        for (j = 0; j < n; j++)
        {
            if (tasks[i].next[j])
            {
                printf("%d ", j);
            }
            
        }
        
        printf("\n");        
    }
    
}

int parse_file(char *fileName)
{
    FILE *grafo = fopen(fileName, "r"); //apertura del fichero en modo lectura

    if (grafo == NULL)
    {
        perror("fopen");    //error en fopen
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_FILE];
    int numTasks = 0;

    while (fgets(line, sizeof(line), grafo))    //leer linea x linea del fichero
    {
        char *ptr_line = line;
        char *token;
        int count = 0;
        int id;
        int num_predecesores;

        while ((token = strsep(&ptr_line, ",")) != NULL)
        {
            switch (count)
            {
            case 0: //nombre-id
                id = atoi(token);
                tasks[id].id = id;
                tasks[id].valid = 1;    //entrada valida
            break;
            
            case 1: //num predecesores
                num_predecesores = atoi(token);
            break;

            default:    //predecesores
                if (count-2 < num_predecesores) //cuando count = 2 se cuenta el primer predecesor
                {
                    int pred_id = atoi(token);
                    tasks[pred_id].next[id] = 1;
                }                
                    
            break;
            }

            count++;
        }
        
        numTasks++;
    }
   
    fclose(grafo); 

   return numTasks;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s<nombre.txt>\n", argv[0]);
    }
    
    char* fileName = argv[1];

    int n = parse_file(fileName);       //parsear fichero

    print_graph(tasks, n);

    return 0;
}