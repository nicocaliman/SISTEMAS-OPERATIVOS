#include <stdio.h>
#include <stdlib.h>

int nelem;
int c;

int main(void)
{
	int *ptr;
	int i;

	c = 37;
	nelem = 127;
	ptr = (int*) malloc(nelem * sizeof(int));
	for (i=0; i < nelem; i++)
		ptr[i] = i;

	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	ptr = &c;
	printf("ptr[0]= %d ptr[13]=%d \n", ptr[0], ptr[13]);

	free(ptr);

}

/*
	ssoo@ssoohost:~/p1so/ejercicio1/5-punteros$ ./punteros3
ptr[0]= 0 ptr[13]=13 
ptr[0]= 37 ptr[13]=0 
free(): invalid pointer
Abortado (`core' generado)

	-Cuando haces ptr = &c;, ptr deja de apuntar a la memoria que fue asignada por malloc(),
	 y en su lugar apunta a la variable c. Esto significa que ptr[0] ahora se refiere a c, y ptr[13] apunta a una posición de memoria fuera 
	 de la variable c, lo que resulta en un comportamiento indefinido. Por lo tanto, el valor de ptr[13] no está definido y 
	 puede producir un resultado inesperado.

	-El error se manifiesta en ejecución. La línea free(ptr); intenta liberar un puntero que no apunta a la memoria dinámica asignada por malloc(). 
	Después de que ptr se reasigna con ptr = &c;, 
	ya no apunta a la memoria asignada por malloc(), así que intentar liberarlo con free(ptr); resulta en un comportamiento indefinido.

	-Tras la asignación ptr = &c;, la zona de memoria reservada por malloc() se pierde, es decir, no hay ninguna referencia a esa memoria,
	 lo que se conoce como "memory leak" (fuga de memoria). No puedes acceder a esa memoria a través de ptr, ya que ahora apunta a c.
	Para liberar la zona de memoria reservada por malloc(), deberías mantener un puntero separado que apunte a esa memoria. Por ejemplo:
*/