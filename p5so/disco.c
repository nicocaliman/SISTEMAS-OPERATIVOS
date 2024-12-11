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
	sleep((rand() % 3) + 1);
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
	
	return 0;
}
