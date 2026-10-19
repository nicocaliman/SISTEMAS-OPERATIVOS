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
El operador que usamos para declarar una variable como un puntero a otro tipo es el asterisco (*)
*/

/*
El operador que usamos para obtener la direccion de memoria de una variable es (&)
*/

/*
El operador que se usa para acceder al valor de la direccion a la que apunta un puntero es el asterisco (*)
*/

/*
El error se produce en ejecucion justo despues de compilar.

En la línea anterior, se ha asignado al puntero ptr una dirección de memoria arbitraria y fija (0x600a48) que no ha sido reservada ni pertenece al espacio de memoria del programa.
Cuando se intenta escribir un valor en esa dirección (*ptr = 13), estás intentando acceder a una zona de memoria protegida o inválida del sistema operativo. Esto provoca una violación de acceso a memoria,
haciendo que el sistema operativo mate el proceso inmediatamente con el clásico error de Segmentation Fault (Fallo de segmento).
*/

