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
 	dst = src;
}

//nueva funcion copia
void copiarArray(int src[], int dst[], int size){
	for(int i=0;i<size;i++)
	{
		dst[i] = src[i];
	}
}


int main()
{
	int A[N] = {4,3,8,5,6,9,0,1,7,2};
	int B[N];

	//copyArray(A,B,N);
	copiarArray(A,B,N);
	imprimeArray(B,N);
}

/*
	En C, los arrays no se pueden copiar utilizando el operador de asignación (=).

Dentro de la función copyArray, los parámetros src y dst son simplemente variables puntero locales. Al escribir dst = src;, lo único que logras es hacer que el puntero local dst apunte a la misma dirección de memoria que src, pero los elementos del array B en el main nunca se modifican 
(de hecho, B permanece sin inicializar, por lo que imprimirlo mostrará valores basura o impredecibles).
*/

/*
Para copiar los elementos correctamente, debes recorrer el array elemento por elemento
*/