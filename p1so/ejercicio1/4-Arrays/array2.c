#include <stdio.h>

#define N 10

// function that prints on the standard output the contents of the array of v of size size
void imprimeArray(int v[],int size)
{
	int i;
	printf("-------------------\n");
	for (i=0;i<size;i++)
		printf("%d ",v[i]);
	printf("\n\n");
}

// function that copies the contents of a source array in a destination array
void copyArray(int src[],int dst[],int size)
{
 	//dst = src;

	for (int i = 0; i < size; i++)
	{
		dst[i] = src[i];
	}
	
}


int main()
{
	int A[N] = {4,3,8,5,6,9,0,1,7,2};
	int B[N];

	copyArray(A,B,N);
	imprimeArray(B,N);
}

/*
ssoo@ssoohost:~/p1so/ejercicio1/4-Arrays$ ./array2
-------------------
0 0 0 0 0 0 0 0 0 0 

  -La copia del array no se realiza correctamente ya que la funcion copyArray() lo que se esta haciendo es que el destino apunte a la direccion de memoria de source
  pero no se esta copiando el contenido de esas direcciones de memoria.

  dst define la direccion del primer elemento del array
  src define la direccion del segundo elemento del array

  -modificamos la funcion copyArray() para realizar correctamente la copia
*/