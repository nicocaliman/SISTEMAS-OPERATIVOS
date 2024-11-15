#include <stdio.h>
#include <stdlib.h>

int c = 7;
int main(void)
{
	int *ptr;
	printf("Address of ptr %p. ptr points to %p. Address of c: %p Value of c %d\n",
			&ptr, ptr, &c, c);

	ptr = &c;
	printf("Address of ptr %p,. ptr points to %p. Address of c: %p Value of c %d\n",
			&ptr, ptr, &c, c);

	*ptr = 4;
	printf("ptr points to %p. Content in the address pointed by ptr: %d Address of c: %p Value of c %d\n",
			ptr, *ptr, &c, c);

	ptr =  (int*) 0x600a48;
	printf("Address of ptr %p. Value of c %d\n", ptr, c);

	*ptr = 13;
	printf("Address of ptr %p. Value of c %d\n", ptr, c);

    return 0;
}

/*
ssoo@ssoohost:~/p1so/ejercicio1/5-punteros$ ./punteros1 
Address of ptr 0x7fff3f659610. ptr points to (nil). Address of c: 0x55a1a7aab010 Value of c 7
Address of ptr 0x7fff3f659610,. ptr points to 0x55a1a7aab010. Address of c: 0x55a1a7aab010 Value of c 7
ptr points to 0x55a1a7aab010. Content in the address pointed by ptr: 4 Address of c: 0x55a1a7aab010 Value of c 4
Address of ptr 0x600a48. Value of c 4
Violación de segmento (`core' generado)


	-El operador que usamos para declarar una variable como un puntero a otro tipo es (*).

	-Para obtener la direccion de memoria de una variable usamos (&).

	-El operador que se utiliza para acceder al contenido de la direccion a la que apunta un puntero es (*).

	-El error que se produce en el codigo es de ejecucion. Esto es debido a que en la linea 19 se asigna una nueva direccion de memoria
	probablemente no valida para el sistema. Al hacer la asignacion al contenido de esa direccion de memoria e la linea 22 se produce segmentatio fault, pues 
	se esta tratando de escribir en una direccion de memoria no valida.

	
*/