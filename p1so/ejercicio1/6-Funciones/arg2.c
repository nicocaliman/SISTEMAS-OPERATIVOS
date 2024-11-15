#include <stdio.h>

/* Structured data type */
struct _complex_ {
	float re;
	float im;
};

/* Forward declaration */
int sum(int *pa, int *pb);
struct _complex_ sumC( struct _complex_ *a,  struct _complex_ *b);	//struct _complex_ *sumC( struct _complex_ *a,  struct _complex_ *b);

int main(void)
{
	int x = 4, y = 5;
	int* ptr = &y;
	struct _complex_ xc = {.re = 1.0, .im = 2.0};
	struct _complex_ yc = {.re = 3.0, .im = 1.0};
	struct _complex_ zc;	//struct _complex_ *zc;


	printf("Complex numbers addition. (%f,%f i) + (%f,%f i) = ",
			xc.re, xc.im, yc.re, yc.im);

	zc = sumC(&xc, &yc);
	printf("(%f,%f i)\n", zc.re, zc.im);

	int total = sum(&x,ptr);
	printf("Integer addition:  x +y = %d + %d = %d \n", x, y, total);
	printf("xc = (%f,%f i)  yc = (%f,%f i) zc = (%f,%f i)\n",
			xc.re, xc.im, yc.re, yc.im, zc.re, zc.im);

	/*
	printf("(%f,%f i)\n", zc->re, zc->im);

	int total = sum(&x,ptr);
	printf("Integer addition:  x +y = %d + %d = %d \n", x, y, total);
	printf("xc = (%f,%f i)  yc = (%f,%f i) zc = (%f,%f i)\n",
	xc.re, xc.im, yc.re, yc.im, zc->re, zc->im);
	*/

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
struct _complex_ *sumC( struct _complex_* a,  struct _complex_* b)
{
	struct _complex_ r;
	r.re = a->re + b->re;
	r.im = a->im + b->im;

	// We modify the first argument
	a->re = 12.5;
	a->im = 13.4;
	return &r;
}*/


/*
ssoo@ssoohost:~/p1so/ejercicio1/6-Funciones$ ./arg2
Violación de segmento (`core' generado)

	-Se modifica el valor de y tras la llamada a sum() porque dentro de la funcion el argumento que se pasa como segundo parametro, su contenido se modifica por 8.
	-A veces se usa . porque significa que se accede al valor directamente desde el struct y -> si se accede al valor del struct a traves de un puntero

	-Por qué el valor de zc pasa a ser incorrecto sin volver a usarlo en el código?

    En la función sumC(), se devuelve un puntero a una variable local r, que es destruida al salir de la función.
	Esto provoca un comportamiento indefinido porque zc apunta a una dirección de memoria que ya no es válida. Por eso, el valor de zc se vuelve incorrecto.


	-ssoo@ssoohost:~/p1so/ejercicio1/6-Funciones$ ./arg2
Complex numbers addition. (1.000000,2.000000 i) + (3.000000,1.000000 i) = (4.000000,3.000000 i)
Integer addition:  x +y = 7 + 8 = 9 
xc = (12.500000,13.400000 i)  yc = (3.000000,1.000000 i) zc = (4.000000,3.000000 i)


*/