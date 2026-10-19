#include <stdio.h>

/* Structured data type */
struct _complex_ {
	float re;
	float im;
};

/* Forward declaration */
int sum(int *pa, int *pb);
//struct _complex_ * sumC( struct _complex_ *a,  struct _complex_ *b);
struct _complex_ sumC( struct _complex_ *a,  struct _complex_ *b);

int main(void)
{
	int x = 4, y = 5;
	int* ptr = &y;
	struct _complex_ xc = {.re = 1.0, .im = 2.0};
	struct _complex_ yc = {.re = 3.0, .im = 1.0};
	//struct _complex_ *zc;
	struct _complex_ zc;


	printf("Complex numbers addition. (%f,%f i) + (%f,%f i) = ",
			xc.re, xc.im, yc.re, yc.im);

	zc = sumC(&xc, &yc);
	//printf("(%f,%f i)\n", zc->re, zc->im);
	printf("(%f,%f i)\n", zc.re, zc.im);

	int total = sum(&x,ptr);
	printf("Integer addition:  x +y = %d + %d = %d \n", x, y, total);
	printf("xc = (%f,%f i)  yc = (%f,%f i) zc = (%f,%f i)\n",
			xc.re, xc.im, yc.re, yc.im, zc.re, zc.im);
	return 0;
}

int sum(int *pa, int *pb)
{
	/* args passed by reference */
	int c = *pa + *pb;

	*pa = 7;
	*pb = 8;
	return c;  /* return by value */
}

/*
struct _complex_ * sumC( struct _complex_* a,  struct _complex_* b)
{
	struct _complex_ r;
	r.re = a->re + b->re;
	r.im = a->im + b->im;

	// We modify the first argument
	a->re = 12.5;
	a->im = 13.4;
	return &r;
}
*/


struct _complex_ sumC( struct _complex_* a,  struct _complex_* b)
{
	struct _complex_ r;
	r.re = a->re + b->re;
	r.im = a->im + b->im;

	// We modify the first argument
	a->re = 12.5;
	a->im = 13.4;
	return r;
}


/*
El valor de y cambia despues de la llamada a sum() debido a que en la declaracion de la funcion se piden 2 punteros a struct. Es decir,
cuando se pasa el struct a la funcion, no se hace por valor sino por referencia (por eso el valor).

Vemos que ptr se pasa a sum() y su valor se modifica por el valor 8 (*ptr = 8).

Cuando se usa . es por que accedemos directamente a los valores de la estructura mediante la propia variable del struct. Sin embargo, cuando usamos -> significa que
estamos usando un puntero a struct (no accedemos directamente a los valores del struct).

Un segmentation fault (o fallo de segmentación) es un error en tiempo de ejecución que ocurre cuando tu programa en C intenta acceder a una zona de memoria que no le pertenece o a la que no tiene permiso de acceso

El valor de zc se corrompe porque la función sumC devuelve la dirección de una variable local (r) alojada en la pila (stack). Al terminar la ejecución de sumC, ese espacio de memoria deja de ser válido, convirtiendo a zc en un puntero colgado (dangling pointer); al ejecutar posteriormente la función sum(), la pila reutiliza esa misma dirección de memoria y sobrescribe el valor previo.

Causa del error

    Ciclo de vida local: struct _complex_ r; es una variable local en sumC. Su memoria en la pila queda marcada como disponible tan pronto la función finaliza.

    Sobrescritura de pila: El primer printf imprime el valor correcto por pura coincidencia (la memoria aún no se ha reutilizado). Al llamar a sum(), se crea un nuevo marco de pila exactamente sobre la dirección donde estuvo r, corrompiendo sus datos antes del segundo printf.

vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/6-Funciones$ ./arg2
Complex numbers addition. (1.000000,2.000000 i) + (3.000000,1.000000 i) = (4.000000,3.000000 i)
Integer addition:  x +y = 7 + 8 = 9 
xc = (12.500000,13.400000 i)  yc = (3.000000,1.000000 i) zc = (4.000000,3.000000 i)



*/