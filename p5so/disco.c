#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 5
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")
#define VIP 1
#define NO_VIP 0

pthread_mutex_t mutex;
pthread_cond_t cond_vips_can_enter, cond_no_vips_can_enter;

//cada hilo tendra 2 argumentos enteros: id(identificador de la tarea que corresponde con el orden de creacion) y isvip(indica si el cliente es vip o no)
struct arg_client
{
	int id;
	int is_vip;
};

int disco_clients = 0;
int vips_at_queue = 0;


void enter_normal_client(int id)
{
	pthread_mutex_lock(&mutex);

	printf("Ha entrado en la cola el cliente no vip con id %d\n", id);

	while (disco_clients >= CAPACITY || vips_at_queue > 0)
	{
		pthread_cond_wait(&cond_no_vips_can_enter, &mutex);
	}

	printf("Ha entrado en la disco el cliente no vip con id: %d\n", id);
	disco_clients++;
	
	pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id)
{
	vips_at_queue++;

	pthread_mutex_lock(&mutex);

	printf("Ha entrado a la cola el cliente vip con id: %d\n", id);

	while (disco_clients >= CAPACITY)
	{
		pthread_cond_wait(&cond_vips_can_enter, &mutex);
	}
	
	printf("Ha entrado a la disco el cliente vip con id: %d\n", id);
	disco_clients++;
	vips_at_queue--;

	pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);	//genera un número aleatorio entre 1 y 3
}

void disco_exit(int id, int isvip)
{
	pthread_mutex_lock(&mutex);
	
	disco_clients--;
	printf("El cliente %s con id: %d ha salido de la disco\n", VIPSTR(isvip), id);

	pthread_mutex_unlock(&mutex);

	if (disco_clients < CAPACITY && vips_at_queue <= 0)
	{
		pthread_cond_signal(&cond_no_vips_can_enter);
	}

	else if(disco_clients < CAPACITY)
	{
		pthread_cond_signal(&cond_vips_can_enter);
	}
	
}

void *client(void *arg)
{
	struct arg_client *client = arg;
	
	if ( client->is_vip )
        enter_vip_client(client->id);
    else
        enter_normal_client(client->id);
    dance(client->id, client->is_vip);
    disco_exit(client->id, client->is_vip);

	free(client);

	pthread_exit(NULL); //terminacion del hilo
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE* fd = fopen(argv[1], "r");	//apertura del fichero con nombre argv[1] en modo lectura

	if (fd == NULL)
	{
		fprintf(stderr, "No se pudo leer el archivo: %s", argv[1]);
		exit(EXIT_FAILURE);
	}

	int M; //numero de clientes a crear

	if(fscanf(fd, "%d", &M) != 1)
	{
		fprintf(stderr, "Error al leer el numero de clientes");
		exit(EXIT_FAILURE);
	}

	int *cliente = malloc(sizeof(int)*M);

	for (int i = 0; i < M; i++)
	{
		if (fscanf(fd, "%d", &cliente[i]) != 1)
		{
			fprintf(stderr, "Error al leer el caracter VIP del cliente %d", i+1);
			fclose(fd);
			exit(EXIT_FAILURE);
		}		
	}

	fclose(fd);	//cierre archivo

	//inicializacion mutex y condiciones
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_vips_can_enter, NULL);
	pthread_cond_init(&cond_no_vips_can_enter, NULL);

	//creacion de los hilos

	pthread_t *threads = malloc(sizeof(pthread_t)*M);
	
	for (int i = 0; i < M; i++)
	{
		struct arg_client *arg = malloc(sizeof(arg));
		arg->id = i+1;
		arg->is_vip = cliente[i];

		pthread_create(&threads[i], NULL, client, arg);
	}

	free(cliente);	//liberar memoria dinamica

	for (int i = 0; i < M; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	free(threads);	

	// Destruir el mutex y las variables de condición
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_vips_can_enter);
	pthread_cond_destroy(&cond_no_vips_can_enter);
	
	return 0;
}

/*
Para implementar la seccion critica de varios hilos de un mismo proceso usamos MUTEX.
	Con esto conseguimos que un solo hilo se encuentre en la seccion critica 
	Si ningun hilo esta ejecutando dentro de la seccion crıtica,
	la decision de que hilo entra en la seccion se hara sobre los procesos
	que desean entrar.

 estructura de codigo:
	<Solicitud de entrada en la seccion crıtica>
	Codigo de la seccion crıtica
	<Indicacion de salida de la seccion crıtica>

 	lock(m);
	<seccion crıtica>
	unlock(m);

 int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

 se utiliza para inicializar un mutex en programas que usan hilos con la biblioteca POSIX Threads (pthread). 
 Un mutex (mutual exclusion) garantiza que solo un hilo pueda ejecutar una sección crítica de código a la vez, evitando condiciones de carrera

 Las condiciones de carrera (race conditions) son un problema en programación concurrente que ocurre cuando el comportamiento o resultado de un programa depende del orden en que se ejecutan hilos 
 o procesos que acceden de manera concurrente a recursos compartidos, como variables, memoria o archivos. Si no se controla adecuadamente, esto puede provocar errores difíciles de reproducir y depurar.

Parámetros
mutex: Puntero al mutex que se va a inicializar. Este debe ser una variable del tipo pthread_mutex_t.
attr: Puntero a los atributos del mutex. Si se pasa NULL, el mutex se inicializa con los atributos predeterminados.

Valor de retorno
	0: La inicialización fue exitosa.
	Otro valor: Un código de error, como:
	EINVAL: Atributos no válidos.
	ENOMEM: Memoria insuficiente.

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);

Parámetros
cond: Puntero a la variable de condición que se va a inicializar. Este debe ser una variable del tipo pthread_cond_t.
attr: Puntero a los atributos de la variable de condición. Si es NULL, se usan los atributos predeterminados.

Valor de retorno
	0: La inicialización fue exitosa.
	Otro valor: Un código de error, como:
	EINVAL: Atributos no válidos.
	ENOMEM: Memoria insuficiente.

se utiliza para inicializar una variable de condición en aplicaciones multihilo utilizando la biblioteca POSIX Threads (pthread).
Las variables de condición permiten a los hilos esperar y ser despertados cuando ocurre un evento específico, facilitando la sincronización entre hilos.


int pthread_mutex_lock(pthread_mutex_t *mutex);

se utiliza en programas multihilo para adquirir un lock (bloqueo) en un mutex. Esto garantiza que solo un hilo puede acceder a una sección crítica protegida por ese mutex a la vez, proporcionando exclusión mutua

Parámetro
mutex: Es un puntero al mutex que se desea bloquear. Este mutex debe haber sido previamente inicializado con pthread_mutex_init o declarado con PTHREAD_MUTEX_INITIALIZER.

Valor de retorno
	0: El bloqueo fue exitoso.
	Otro valor: Un código de error:
	EINVAL: El mutex no es válido o no fue inicializado correctamente.
	EDEADLK: Se detectó un deadlock (por ejemplo, si el hilo intenta bloquear un mutex que ya posee y no es recursivo).

 Comportamiento
Si el mutex está desbloqueado, el hilo adquiere el lock y continúa.
Si el mutex ya está bloqueado, el hilo se suspende y espera hasta que el mutex sea desbloqueado por otro hilo.

int pthread_mutex_unlock(pthread_mutex_t *mutex);

se utiliza para liberar un lock (bloqueo) previamente adquirido en un mutex. Esto permite que otros hilos puedan adquirir el mutex y entrar en la sección crítica protegida por este.

Parámetro
mutex: Es un puntero al mutex que se desea desbloquear. Este mutex debe haber sido bloqueado previamente por el mismo hilo utilizando pthread_mutex_lock o pthread_mutex_trylock.

Valor de retorno
0: Desbloqueo exitoso.
Otro valor: Un código de error:
EPERM: El hilo que intenta desbloquear el mutex no lo posee.
EINVAL: El mutex no es válido o no fue inicializado.

Comportamiento
Libera el mutex: El mutex pasa al estado desbloqueado, permitiendo que otros hilos lo bloqueen.
Hilos en espera: Si hay hilos esperando el mutex, uno de ellos se desbloquea y adquiere el lock.
Propietario único: Solo el hilo que bloqueó el mutex puede desbloquearlo. Intentar desbloquearlo desde otro hilo genera un error.

Un mutex (abreviatura de mutual exclusion, o exclusión mutua) es un mecanismo de sincronización utilizado en programación concurrente para evitar condiciones de carrera (race conditions) y garantizar el acceso exclusivo a recursos compartidos entre múltiples hilos o procesos.


 int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
 
es parte de las primitivas de sincronización en programación multihilo y se utiliza para que un hilo espere una señal en una condición. Esta función siempre debe usarse junto con un mutex para evitar condiciones de carrera.

Parámetros
cond: Un puntero a una variable de condición (pthread_cond_t) que se utiliza para notificar a los hilos.
mutex: Un puntero al mutex (pthread_mutex_t) asociado con la condición.

Comportamiento
Desbloquea el mutex:

Cuando un hilo llama a pthread_cond_wait, libera automáticamente el mutex asociado, permitiendo que otros hilos lo adquieran.
Espera la señal:

El hilo queda en estado de espera (bloqueado) hasta que otro hilo envíe una señal con pthread_cond_signal o pthread_cond_broadcast.
Readquiere el mutex:

Cuando el hilo es despertado, antes de continuar su ejecución, vuelve a bloquear el mutex automáticamente.


int pthread_cond_signal(pthread_cond_t *cond);
 
 se utiliza para desbloquear a uno de los hilos que están esperando en una condición de tipo pthread_cond_t. Es parte de las primitivas de sincronización en programación multihilo y permite coordinar el trabajo entre hilos mediante señales.
Parámetro
cond: Es un puntero a una variable de condición (pthread_cond_t) previamente inicializada. Representa la condición que se desea señalar.

Comportamiento
Señala un hilo:

Despierta a uno de los hilos que están bloqueados en pthread_cond_wait para la condición especificada.
Si no hay hilos esperando en la condición, la señal se pierde y no tiene efecto.
No libera el mutex:

pthread_cond_signal no afecta el estado de ningún mutex asociado. Es responsabilidad del hilo que envía la señal manejar el mutex de manera adecuada.

Uso de Variables de Condici ́on Uso de Variables de Condici ́on

Hilo que espera mientras se cumpla una condicion:
	lock(mutex);
	while (<conditional expresion>)
	wait(cond_var, mutex);
	<acciones restantes en secci ́on cr ́ıtica>
	unlock(mutex);
	<acciones deseadas fuera de secci ́on cr ́ıtica>
	
 Se ̃nalizaci ́on desde otro hilo:
	lock(mutex);
	<operaciones que afectan a la expresi ́on condicional>
	signal(cond_var);
	<otras operaciones protegidas>
	unlock(mutex);

Es decir:
wait: siempre en un bucle while
wait: siempre con el mutex cogido
signal: mejor con el mutex cogido

*/
