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
Al intentar leer ptr[13], el programa accede a la posición equivalente a 13 enteros más allá de donde está c. Como c es una única variable, leer más allá de ella provoca un comportamiento indefinido (undefined behavior), arrojando un valor arbitrario o basura que se encuentre en esa zona de la memoria
*/

/*
Se manifiesta en ejecución. El compilador no detecta el error porque sintácticamente es correcto pasar cualquier puntero a free().
Se intenta liberar con free() una dirección de memoria (&c) que pertenece a las variables globales/estáticas, no a la memoria dinámica reservada por malloc(). Esto provoca un fallo de segmentación o la corrupción del gestor de memoria en tiempo de ejecución. 
Además, hay un segundo error grave: fuga de memoria, ya que se sobrescribe el puntero a la memoria del heap sin haberla liberado antes
*/

/*
Se produce una fuga de memoria. El bloque de 127 enteros sigue reservado en el sistema, pero el programa pierde la única referencia (ptr) que tenía para localizarlo.

Una vez sobreescrito ptr, ya no es posible acceder a esa zona de memoria.

Para poder liberarla correctamente, deberías haber guardado el valor original de ptr en una variable auxiliar (por ejemplo, int *aux = ptr;) antes de cambiar ptr = &c;, y luego hacer free(aux); antes de que finalice el programa.
*/