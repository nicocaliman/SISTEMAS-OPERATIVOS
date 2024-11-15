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
ssoo@ssoohost:~/p1so/ejercicio1/4-Arrays$ ./array1
Address of list 0x7ffc4ddad9b0 Address of list[0]: 0x7ffc4ddad9b0 Address of list[1]: 0x7ffc4ddad9b4. Sizeof list 20 
Address of the array: 0x7ffc4ddad9b0 Sizeof array 8 
Array initialized

next: 0 next: 1 next: 2 next: 3 next: 4 
-------------------------
Address of the array: 0x7ffc4ddad9b0 Sizeof array 8 
Array initialized

next: 0 next: 2 next: 4 next: 6 next: 8 
-------------------------

	- No es necesario escribir &list en la linea 12, porque el propio nombre del array apunta a la primera direccion del array(list[0]) que 
	es la primera direccion que se reserva para almacenar el primer elemento del array

	-En la direccion de list tenemos almacenado el primer elemento del array.

	-Hay que pasar el tamanio del array a la funcion init_array() porque el array solo tiene informacion acerca de la direccion del primer elemento del array
	pero no sabe cuantos elementos tiene.

	-El array de main tiene un tamanio de 20 bytes debido a que al inicializarlo con tamanio 5, se estan reservando 5 posicion * 4Bytes que ocupa un dato de tipo int.
	Es decir, 5*4 = 20 Bytes de tamanio total se reservan para el array del main.

	Por otro lado, en la funcion init_array1(), al pasar el nombre del array como argumento, lo que estamos haciendo es pasar un puntero cuyo tamanio son 8 Bytes.

	-No hace falta pasar como argumento el tamanio del array, porque ya se pasa como argumento un array con tamanio definido

	-El tamanio del array de la funcion init_array2() es 8(estoy pasando un puntero), mientras que el del main() es 20(5 elementos * 4 Bytes).
*/