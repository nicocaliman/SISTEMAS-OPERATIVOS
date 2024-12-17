#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define THREADS_NUMBER 10

struct tharg
{
	int numHilo;
	char prioridad;
};

void *thread_usuario(void *arg)
{
	//obtener los argumentos desde el puntero void *
	struct threadArgs *thr_args = (struct threadArgs *) arg;

	//copiar argumentos en variables locales
	int nThread = thr_args->numHilo;
	char prioridad = thr_args->prioridad;

	//liberar memoria dinamica
	free(thr_args);

	//obtener id del hilo
	pthread_t id = pthread_self();	//obtener id del hilo al que hemos invocado

	printf("ID hilo: %lu Numero hilo: %d Prioridad: %c\n", id, nThread, prioridad);

	pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
	pthread_t threads[THREADS_NUMBER];
	//struct threadArgs *thr_usr = malloc(sizeof(struct threadArgs));

	for (int i = 0; i < THREADS_NUMBER; i++)
	{
		struct threadArgs *thr_usr = malloc(sizeof(struct threadArgs));	//usar memoria dinamica para crear una variable que guarde los argumentos de cada hilo

		thr_usr->numHilo = i+1;	//numero de hilo
		thr_usr->prioridad = ((i+1) % 2 == 0) ? 'P' : 'N';	//par = prioritario impar = no prioritario

		if(pthread_create(&threads[i], NULL, thread_usuario,thr_usr) != 0)	//pthread_create() == crear un nuevo hilo; el hilo empieza invocando al rutina thread_usuario(), el ultimo parametro es lo que se pasa como argumento a la rutina
		{
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < THREADS_NUMBER; i++)
	{
		if(pthread_join(threads[i], NULL) != 0)	//
		{
			perror("error waiting thread");
			exit(EXIT_FAILURE);
		}
	}	

	return 0;
}

/*
la linea comentada se refiere a Probar a crear solamente una variable para el argumento de todos los hilos, 
dándole el valor correspondiente a cada hilo antes de la llamada a pthread_create. 

Cada hilo accede a thr_usr mientras otros hilos tambien estan escribiendo en ella, lo que lleva a comportamientos impredecibles.
Ademas, una vez los hilos comienzan a ejecutarse, no hay garantia de que mantendran los valores originales de thr_args, ya que podrian estar leyendo 
y escribiendo en la misma variable al mismo tiempo.

Por lo que es mejor asignar una variable separada por cada HILO.

int pthread_join(pthread_t thread, void **retval);
*/

/*
permite a un hilo esperar a que otro hilo termine su ejecución. Es útil para sincronizar hilos y 
asegurar que el hilo principal (o cualquier otro hilo) espere el resultado de otro hilo antes de continuar.
*/
