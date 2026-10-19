#include <stdio.h>

#define N 5

void init_array(int array[], int size) ;
void init_array2(int array[N]);

int main(void)
{
	int i,list[N];
	printf("Address of list %p Address of list[0]: %p Address of list[1]: %p. Sizeof list %lu \n",
			list, &list[0], &list[1], sizeof(list));

	init_array(list, N);
	for (i = 0; i < N; i++)
		printf("next: %d ", list[i]);
	printf("\n-------------------------\n");

	init_array2(list);
	for (i = 0; i < N; i++)
		printf("next: %d ", list[i]);
	printf("\n-------------------------\n");
}

void init_array(int array[], int size)
{
	int i;
	printf("Address of the array: %p Sizeof array %lu \n", array, sizeof(array));
	for (i = 0; i < size; i++)
		array[i] = i;
	printf("Array initialized\n\n");
}

void init_array2(int array[N])
{
	int i;
	printf("Address of the array: %p Sizeof array %lu \n", array, sizeof(array));
	for (i = 0; i < N; i++)
		array[i] = i*2;
	printf("Array initialized\n\n");
}

/*
El nombre que le damos al array almacena directamente (apunta) la primera direccion de la primera posicion del array. 
(un puntero que apunta a su primer elemento (&list[0]))
*/

/*
Es necesario pasar el tamanio que tendra el array porque el nombre del array unicamente nos pasa la direccion del la primera posicion del array, no nos da informacion sobre el tamanio que 
tiene que tener el array
*/

/*
En main(), list es un array completo de 5 enteros ($5 \times 4 = 20$ bytes).Dentro de init_array, el parámetro int array[] se interpreta internamente como un puntero (int *array). Por lo tanto, sizeof(array) calcula el tamaño que ocupa el puntero en sí (8 bytes en sistemas de 64 bits), no el espacio total del array.
*/

/*
Aunque escribas int array[N] en la firma de la función, el compilador ignora ese número entre corchetes y trata al parámetro exactamente igual que a un puntero (int *array). Si no se pasa como argumento, es porque la función recurre directamente a la macro global N definida al principio del archivo (#define N 5).
*/

/*
No coincide. Al igual que ocurre en init_array, el parámetro array en init_array2 es un puntero enmascarado. Por lo tanto, sizeof(array) devolverá los bytes del puntero (8 bytes), mientras que en main() seguirá devolviendo el tamaño total del array completo (20 bytes).
*/