#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAXPROC 16
#define MAX_LINE_FILE 100

void wait_for_predecessors(int id);
void notify_sucessors(int id);

struct task
{
    int valid;  //entrada valida(1), no valida(0)
    int next[MAXPROC];  //array de booleanos para sucesores, next[i] = 1 si sale una arista hacia el nodo i
    int id; //nombre de la tarea, que corresponde a su posicion en el array de nodos
    pthread_mutex_t m;  //mutex
    pthread_cond_t cond;    //variable de condicion
    int completed_predecessors;
    int num_predecessors;
};

struct tharg
{
    int id;
};

struct task tasks[MAXPROC]; //variable global

void *task_body(void *arg)
{
    int id = *(int*)arg;
    wait_for_predecessors(id);  //bloqueante, el hilo espera que sus predecesores le notifiquen su finalizacion
    printf("Task %d running \n", id);   //cuerpo real de la tarea
    notify_sucessors(id);   //sincronizacion, aviso a los sucesores de que esta tarea ha terminado
    
    return NULL;
}

//esperar la notificacion de todos sus predecesores en el grafo
void wait_for_predecessors(int id)
{
    struct task *task = &tasks[id];

    pthread_mutex_lock(&task->m);
    while (task->completed_predecessors < task->num_predecessors)
    {
        pthread_cond_wait(&task->cond, &task->m);
    }
    pthread_mutex_unlock(&task->m);
}

//notifica a los sucesores que esta tarea ha terminado
void notify_sucessors(int id)
{
    struct task *task = &tasks[id];

    int i;
    for (i = 0; i < MAXPROC; i++)
    {
        if (task->next[i])
        {
            pthread_mutex_lock(&tasks[i].m);
            tasks[i].completed_predecessors++;

            if (tasks[i].completed_predecessors == tasks[i].num_predecessors)
            {
                pthread_cond_signal(&tasks[i].cond);
            }          

            pthread_mutex_unlock(&tasks[i].m);   
        }       
    }
    
}

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

    int i;
    for ( i = 0; i < MAXPROC; i++)
    {
        tasks[i].valid = 0;
        pthread_mutex_init(&tasks[i].m,NULL);
        pthread_cond_init(&tasks[i].cond,NULL);
        tasks[i].num_predecessors = 0;
        tasks[i].completed_predecessors = 0;
    }
    


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

    pthread_t threads[MAXPROC];

    int i;

    for (i = 0; i < n; i++) //crear un hilo por cada tarea
    {
        if(tasks[i].valid)
            pthread_create(&threads[i], NULL, task_body, &tasks[i].id);
    }

    for (i = 0; i < n; i++) //crear un hilo por cada tarea
    {
        if(tasks[i].valid)
            pthread_join(threads[i], NULL);
    }
    
    for (i = 0; i < MAXPROC; i++)
    {
        if(tasks[i].valid)
        {
            pthread_mutex_destroy(&tasks[i].m);
            pthread_cond_destroy(&tasks[i].cond);
        }
    }
    
    return 0;
}