// cocinero.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define M 10

int finish = 0;

sem_t *m, *empty, *full;

int *caldero; // Memoria compartida

// El cocinero sólo puede invocarla si el caldero está vacío.
void putServingsInPot(int servings)
{
	// Espera hasta que el caldero este vacío.
	sem_wait(empty);
	
	// Llena el caldero.
	*caldero += M;

	printf("Chef put servings in pot, current servings: %d\n", *caldero);

	// Indica a los salvajes que el caldero esta lleno.
	sem_post(full);
}

void cook(void)
{
	unsigned long id = (unsigned long) getpid();
	
	printf("Chef with id: %lu is working\n", id);
	while(!finish) 
	{
		putServingsInPot(M);
	}
}

void handler(int signo)
{
	finish = 1;
}

int main(int argc, char *argv[])
{
	// Creación de los recursos compartidos necesarios
	int shm_fd = shm_open("/caldero", O_CREAT | O_RDWR, 0666);
	
	if (shm_fd == -1)
	{
	perror("Error al crear memoria compartida");
	exit(EXIT_FAILURE);
	}
	
	ftruncate(shm_fd, sizeof(int));

	caldero = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if(caldero == MAP_FAILED)
	{
		perror("Error al mapear memoria compartida");
		exit(EXIT_FAILURE);
	}
	
	// Establece el manejador de la señal SIGTERM

	struct sigaction sT = {0};
	sT.sa_flags = 0;
	sT.sa_handler = &handler;

	if (sigaction(SIGTERM, &sT, NULL) == -1)
	{
		perror("sigaction SIGTERM");
		exit(EXIT_FAILURE);
	}

	// Establece el manejador de la señal SIGINT
	struct sigaction sI = {0};
	sI.sa_flags = 0;
	sI.sa_handler = &handler;

	if (sigaction(SIGINT, &sI, NULL) == -1)
	{
		perror("sigaction SIGINT");
		exit(EXIT_FAILURE);
	}

	// Inicialización de los semáforos
	m = sem_open("/m", O_CREAT, 0600, 1);
	empty = sem_open("/empty", O_CREAT, 0600, 1);
	full = sem_open("/full", O_CREAT, 0600, 0);

	// Comienzo del programa
	cook();
	sem_unlink("/m");
	sem_unlink("/empty");
	sem_unlink("/full");
	shm_unlink("/caldero");
	
	return 0;
}