#include <stdio.h>

/* Structured data type */
struct _complex_ {
	float re;
	float im;
};

/* Forward declaration */
//int sum(int a, int b);
//struct _complex_  sumC( struct _complex_ a,  struct _complex_ b);

int main(void)
{
	int x = 4, y = 5;
	struct _complex_ xc = {.re = 1.0, .im = 2.0};
	struct _complex_ yc = {.re = 3.0, .im = 1.0};
	struct _complex_ zc;

	zc = sumC(xc, yc);

	int total = sum(x, y);

	printf("Complex numbers addition. (%f,%f i) + (%f,%f i) =(%f,%f i)\n",xc.re,xc.im,yc.re,yc.im,zc.re,zc.im);
	printf("Integer addition:  x +y = %d + %d = %d \n",x,y, total);
	return 0;
}

int sum(int x, int y)
{
	int c;
	c = x +y;
	x = 7;
	y =3;
	return c;
}


struct _complex_  sumC( struct _complex_ a,  struct _complex_ b)
{
	struct _complex_ r;
	r.re = a.re + b.re;
	r.im = a.im + b.im;

	// We modify the first argument
	a.re = 12.5;
	a.im = 13.4;
	return r;
}

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/6-Funciones$ ./arg1 
Complex numbers addition. (1.000000,2.000000 i) + (3.000000,1.000000 i) =(4.000000,3.000000 i)
Integer addition:  x +y = 4 + 5 = 9 

El valor de xc no se modifica tras la llamada a sumC porque el struct se pasa por valor. Los valores de la estructura se modifican de manera local dentro de
la funcion sumC

Si comentamos as 2 declaraciones de sum y sumC nos da los siguientes errores en terminal:

vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/6-Funciones$ gcc -o arg1 arg1.c
arg1.c: In function ‘main’:
arg1.c:20:14: error: implicit declaration of function ‘sumC’ [-Wimplicit-function-declaration]
   20 |         zc = sumC(xc, yc);
      |              ^~~~
arg1.c:20:14: error: incompatible types when assigning to type ‘struct _complex_’ from type ‘int’
arg1.c:22:21: error: implicit declaration of function ‘sum’ [-Wimplicit-function-declaration]
   22 |         int total = sum(x, y);
      |                     ^~~
arg1.c: At top level:
arg1.c:39:19: error: conflicting types for ‘sumC’; have ‘struct _complex_(struct _complex_,  struct _complex_)’
   39 | struct _complex_  sumC( struct _complex_ a,  struct _complex_ b)
      |                   ^~~~
arg1.c:20:14: note: previous implicit declaration of ‘sumC’ with type ‘int()’
   20 |         zc = sumC(xc, yc);
      |              ^~~~


En arg2 pasalo siguiente:


vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/6-Funciones$ gcc -o arg2 arg2.c
arg2.c: In function ‘sumC’:
arg2.c:54:16: warning: function returns address of local variable [-Wreturn-local-addr]
   54 |         return &r;
      |                ^~
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/6-Funciones$ ./arg2
Segmentation fault         (core dumped) ./arg2

*/
