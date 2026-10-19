#include <stdio.h>

char a = 122;
int b = 41;

int main()
{
	printf("a = %d a = %c \n", a, a);
	a += 6;
	printf("a = %d a = %c b=%d  b=%c\n", a, a, b, b);
	printf("Size of int: %lu\n", sizeof(int) );
	printf("Size of char: %lu\n", sizeof(char) );
	printf("Size of float: %lu\n", sizeof(float) );
	printf("Size of double: %lu\n", sizeof(double) );
	printf("Size of long: %lu\n", sizeof(long) );
	printf("Size of short: %lu\n", sizeof(short) );
	printf("Size of void*: %lu\n", sizeof(void*) );

}

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/3-Tams$ ./main1 
a = 122 a = z 
a = -128 a = � b=41  b=)
Size of int: 4
Size of char: 1
Size of float: 4
Size of double: 8
Size of long: 8
Size of short: 2
Size of void*: 8

%d: espera un decimal
%c:espera un char. 122 equivale al caracter 'z' en ASCII

Size of char: 1 (el tamanio de un char equivale a 1 byte)

El rango de tamanio de un char va desde -128,127. 
Al sumarle 6 al 122 = 128, el tamanio sale del intervalo lo que produce un overflow (desbordamiento).

Debido a cómo las computadoras almacenan los números negativos (complemento a dos), el valor da la vuelta (wrap around) y cae en el extremo negativo, convirtiéndose en -128.


long: Está diseñado para almacenar números enteros grandes utilizando representación binaria estándar.
double: Está diseñado para almacenar números reales (con decimales) utilizando el estándar IEEE 754 (que divide los bits en signo, exponente y mantisa). Esto le permite manejar números con decimales y rangos astronómicamente grandes o pequeños que un entero no podría representar de la misma manera

*/