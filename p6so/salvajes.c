// salvajes.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>

#define NUMITER 3

sem_t *m, *empty, *full;

int *caldero; // Memoria compartida

// Los salvajes no pueden invocarla si el caldero está vacío.
int getServingsFromPot(void)
{
	// El salvaje espera a su turno.
	sem_wait(m);
	
	// Si el caldero esta vacío.
	if (*caldero == 0)
	{
		// Notifica que se el caldero esta vacío.
		sem_post(empty);
		
		// Espera hasta que el caldero este lleno.
		sem_wait(full);
	}

	// El salvaje agarra su ración.
	(*caldero)--;
	
	// El salvaje acabo su turno.
	sem_post(m);
}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages(void)
{
	int i;
	
	for (i = 0; i < NUMITER; i++)
	{
		getServingsFromPot();
		eat();
	}
}

int main(int argc, char *argv[])
{
	// Apertura de los recursos compartidos
	int shm_fd = shm_open("/caldero", O_RDWR, 0);
	
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

	m = sem_open("/m", O_EXCL);
	empty = sem_open("/empty", O_EXCL);
	full = sem_open("/full", O_EXCL);

	// Comienzo del programa
	savages();
	sem_unlink("/m");
	sem_unlink("/empty");
	sem_unlink("/full");

	return 0;
}