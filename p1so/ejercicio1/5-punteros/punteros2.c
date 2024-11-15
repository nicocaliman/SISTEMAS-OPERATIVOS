#include <stdio.h>
#include <stdlib.h>

int nelem;

int main(void)
{
	int *ptr;
	int * ptr2;

	nelem = 127;
	ptr = (int*) malloc(nelem*sizeof(int));
	*ptr = 5;
	ptr[0] = 7;
	ptr2 = ptr;

	printf("Address pointed by ptr %p. Memory content at that address: %d \n",
			ptr, *ptr);

	ptr[1] = 10;
	printf("Address of the element ptr[1] %p. Memory content at that address: %d \n",
			&ptr[1], ptr[1]);

	ptr2++;
	*ptr2 = 15;
	printf("Address of the element ptr[1] %p. Memory content at that address: %d \n",
			&ptr[1], ptr[1]);

	free(ptr);
	*ptr = 3;
	printf("Address pointed by ptr %p. Memory content at that address: %d \n",
			ptr, *ptr);
	return 0;
}

/*
ssoo@ssoohost:~/p1so/ejercicio1/5-punteros$ ./punteros2
Address pointed by ptr 0x55dcbf8a02a0. Memory content at that address: 7 
Address of the element ptr[1] 0x55dcbf8a02a4. Memory content at that address: 10 
Address of the element ptr[1] 0x55dcbf8a02a4. Memory content at that address: 15 
Address pointed by ptr 0x55dcbf8a02a0. Memory content at that address: 3

	-Con malloc se reserva en memoria 127 elementos * 4Bytes cada uno = 508Bytes

	-La primera direccion de dicha zona reservada es 0x55dcbf8a02a0 y la ultima es 
	
		Direccioˊn del uˊltimo byte=ptr+126

		Esto se debe a que el primer elemento es ptr[0] y el último es ptr[126]. En bytes, esto sería:

		Direccioˊn del uˊltimo byte=ptr+126×sizeof(int)Direccioˊn del uˊltimo byte=ptr+126×sizeof(int)

	-Se imprime 7 en vez de 5 porque primero se asigna inmediatamente 7 despues de haberler asignado 5

	-ptr1 se modifica pq ptr2 apunta a su direccion de memoria(a la primera ptr1[0]), por tanto ptr2++ = direccion de ptr1[1]
	*ptr2 = 15 modifica entonces el contenido de ptr1[1].

	-escribir el valor 13 en la dirección correspondiente a ptr[100].

			ptr[100] = 13
			*(ptr+100) = 13

	-El error se manifiesta en ejecución. La línea *ptr = 3; después de free(ptr); intenta acceder a memoria que ya ha sido liberada.
	 Esto es un comportamiento indefinido en C y puede provocar errores como un fallo de segmentación o resultados inesperados. 
	 Después de liberar la memoria con free(ptr);, no se debe intentar acceder a esa memoria.

*/