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
ssoo@ssoohost:~/p1so/ejercicio1/3-Tams$ ./main1 
a = 122 a = z 
a = -128 a = � b=41  b=)
Size of int: 4
Size of char: 1
Size of float: 4
Size of double: 8
Size of long: 8
Size of short: 2
Size of void*: 8



	-El primer printf() imprime valores diferentes para la variable a porque el identificador %d hacer referencia a que se 
	ha de devolver un valor decimal, mientras que el modificador %c quiere mostrar por pantalla un tipo char.

	122 es el caracter z, que se imprime en pantalla (122 en ASCII es 'z')

	-Un char ocupa 1 byte

	-El valor de a cambia tanto (hasta -128) debido a que 122 + 6 = 128. El valor 128 sobrepasa el rango de valores que
	soporta el tipo char ([-128,127])

	-double y long ocupan lo mismo. Sin embargo, long se usa para representar numeros enteros que no tengan decimales, mientras 
	que double representa tipos de datos de punto flotante con decimales.

*/