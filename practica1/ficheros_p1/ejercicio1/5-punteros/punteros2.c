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
	127*4 = 508 bytes de memoria se reservan

	La primera direccion de esta reserva es 0x5d29d4150010 y la ultima seria 0x5d29d4150010 + 508 bytes - 1 (0x5d29d4150010)
	
*/

/*
la instrucción inmediatamente posterior es ptr[0] = 7;, la cual sobrescribe el valor anterior en la misma posición de memoria (dado que ptr[0] y *ptr se refieren exactamente al mismo lugar).
*/

/*
Porque ptr2 = ptr; hace que ptr2 apunte al inicio del bloque. Al ejecutar ptr2++;, el puntero avanza una posición de tipo int (4 bytes), apuntando ahora exactamente a ptr[1]. 
Por lo tanto, *ptr2 = 15; modifica directamente el valor de ptr[1].
*/

/*
Mediante la notación de arreglos: ptr[100] = 13;

Mediante aritmética de punteros: *(ptr + 100) = 13;
*/

/*
El error se encuentra al final con las líneas free(ptr); y *ptr = 3;. Se trata de un fallo de uso de memoria tras ser liberada (use-after-free). Este error se manifiesta en ejecución
*/