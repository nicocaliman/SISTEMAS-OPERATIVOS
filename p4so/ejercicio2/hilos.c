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
*/

/*
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

Parámetros
thread:

Un puntero a una variable de tipo pthread_t que recibe el identificador del nuevo hilo creado.
Este identificador se puede usar más tarde con otras funciones de la biblioteca pthread para manipular el hilo (por ejemplo, para esperar a que termine o para cancelarlo).
attr:

Un puntero a un objeto de tipo pthread_attr_t que define los atributos del hilo (por ejemplo, si el hilo será de prioridad alta o baja, o si debe ser creado como hilo desasociado).
Si se pasa NULL, se utilizan los valores por defecto para los atributos del hilo.
start_routine:

Un puntero a la función que el hilo ejecutará una vez que se haya creado. La función debe tener la siguiente firma:

void *func(void *arg);
arg es un puntero a los datos que se pasan a la función. Si no se necesita pasar ningún dato, se puede usar NULL.
arg:

Un puntero a cualquier tipo de datos que se pasará como argumento a la función start_routine. Este parámetro puede ser NULL si no se requiere pasar información al hilo.

Valor de retorno
pthread_create devuelve 0 si el hilo fue creado correctamente.
Si ocurre un error, devuelve un número negativo (por ejemplo, ENOMEM si no hay suficiente memoria).

int pthread_join(pthread_t thread, void **retval);

permite a un hilo esperar a que otro hilo termine su ejecución. Es útil para sincronizar hilos y 
asegurar que el hilo principal (o cualquier otro hilo) espere el resultado de otro hilo antes de continuar.

Parámetros
thread: Es el identificador del hilo que se desea esperar. Este identificador se obtiene cuando se crea un hilo con pthread_create.
retval: Es un puntero a una variable donde se almacenará el valor devuelto por el hilo que termina (normalmente definido en pthread_exit o el valor que devuelve la función ejecutada por el hilo). Si no te interesa este valor, puedes pasar NULL.

Retorno
Devuelve 0 si la llamada se realizó con éxito.
En caso de error, devuelve un valor distinto de 0, como:
ESRCH: El hilo no existe.
EINVAL: El hilo no es "joinable" (por ejemplo, fue creado con la opción PTHREAD_CREATE_DETACHED).
EDEADLK: Se detectó un deadlock al intentar realizar la operación.
Comportamiento
La llamada a pthread_join bloquea la ejecución del hilo llamante hasta que el hilo especificado en thread finaliza.
Es comúnmente usada para asegurarse de que un hilo ha completado su tarea antes de que el programa finalice o se proceda con otras operaciones dependientes.
*/
