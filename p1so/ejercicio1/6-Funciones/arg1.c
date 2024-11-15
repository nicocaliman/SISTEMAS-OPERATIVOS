#include <stdio.h>

/* Structured data type */
struct _complex_ {
	float re;
	float im;
};

/* Forward declaration */
int sum(int a, int b);
struct _complex_  sumC( struct _complex_ a,  struct _complex_ b);

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
ssoo@ssoohost:~/p1so/ejercicio1/6-Funciones$ ./arg1 
Complex numbers addition. (1.000000,2.000000 i) + (3.000000,1.000000 i) =(4.000000,3.000000 i)
Integer addition:  x +y = 4 + 5 = 9 

	-El valor de xc no se modifica tras la llamada a sumC porque en C, 
	los argumentos de las funciones se pasan por valor de forma predeterminada. 
	Esto significa que cuando llamas a sumC(xc, yc), se crea una copia de xc y yc dentro de la función sumC. 
	Así, cualquier modificación que hagas a a y b dentro de sumC no afecta a las variables originales xc y yc en main.

	En la función sumC, se modifica la copia a (la cual es una copia de xc), cambiando a.re y a.im, pero esto no afecta el valor de xc en main. 
	La información de xc permanece sin cambios después de la llamada.

	-Si comentas las declaraciones adelantadas de sum() y sumC() (las líneas que dicen int sum(int a, int b); y struct _complex_ sumC(struct _complex_ a, struct _complex_ b);), 
	el compilador no tendrá información sobre estas funciones antes de su uso en main(). 
	Al compilar de nuevo, obtendrás errores de compilación que indican que las funciones sum y sumC no están declaradas.

*/