#include <stdio.h>


#define ARRAY_SIZE  10
#define  ARRAY2_SIZE 7

int a = 7;
unsigned long b = 8;
short c;
char x;
char* pc;

int array1[ARRAY_SIZE];
int array2[ARRAY2_SIZE];

char* str1    = "STRING OF CHARACTERS";
char str2[]   = "STRING OF CHARACTERS";
int main()
{
	pc = &x;
	a = 16;
	printf("Address of a: %p Size: %lu \n",&a,sizeof(a));
	printf("Address of b: %p Size: %lu \n",&b,sizeof(b));
	printf("Address of c: %p Size: %lu \n",&c,sizeof(c));
	printf("Address of x: %p Size: %lu \n",&x,sizeof(x));
	printf("Address of pc: %p Address pointed by pc: %p Size: %lu \n",
			&pc, pc, sizeof(pc));
	printf("Address of array: %p Addres of elemnt 0: %p Size of the array: %lu \n",
			array1, &array1[0], sizeof(array1));
	printf("Address of str1: %p Address pointed by it: %p Size: %lu \n",
			&str1, str1, sizeof(str1));
	printf("Address of str2: %p Address pointed by it: %p Size: %lu \n",
			&str2, str2, sizeof(str2));
    return 0;
}

/*
Es un problema de compilación. El código fallará antes de llegar a ejecutarse.
*/

/*
int array2[a];

El tamanio con el que se declara el array2 no es constante. a es una variable de tipo int, no una macro como ARRAY_SIZE.
*/

/*
Se utiliza para imprimir valores devueltos por el operador sizeof (tipo size_t), 
ya que este operador siempre devuelve un número entero sin signo que representa tamaños de memoria en bytes
*/

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/3-Tams$ ./main2
Address of a: 0x5ae7d6c39010 Size: 4 
Address of b: 0x5ae7d6c39018 Size: 8 
Address of c: 0x5ae7d6c39060 Size: 2 
Address of x: 0x5ae7d6c39062 Size: 1 
Address of pc: 0x5ae7d6c39068 Address pointed by pc: 0x5ae7d6c39062 Size: 8 
Address of array: 0x5ae7d6c39080 Addres of elemnt 0: 0x5ae7d6c39080 Size of the array: 40 
Address of str1: 0x5ae7d6c39038 Address pointed by it: 0x5ae7d6c37008 Size: 8 
Address of str2: 0x5ae7d6c39020 Address pointed by it: 0x5ae7d6c39020 Size: 21 

pc apunta a la direccion  0x5ae7d6c39062 que coincide con la direccion de la variable x

Tamaños: No coinciden. x es un char y ocupa 1 byte, mientras que pc es un puntero (char*) que, en sistemas de 64 bits, ocupa 8 bytes
*/

/*
No directamente, sizeof(array1) devuelve el tamaño total del array en bytes, no el número de elementos
Sabemos que el numero de elementos del array son 10. Como es un array de tipo int, sabemos que un int en memoria ocupa 4 bytes.
Por tanto, 4bytes*10elementos = 40
*/

/*
No.

    str1 es un puntero que apunta a una cadena literal de caracteres (generalmente almacenada en la sección de memoria de solo lectura).

    str2 es un array que almacena una copia propia de esa misma cadena en otra zona de memoria (en la sección de datos globales).

    Por lo tanto, apuntan a contenidos idénticos, pero en ubicaciones de memoria diferentes.
*/

/*
sizeof(str1) da 8 bytes porque str1 es estrictamente una variable puntero (char*), y los punteros en 64 bits miden 8 bytes.

sizeof(str2) da 22 bytes porque str2 es un array de caracteres. El operador sizeof aplicado a un array calcula el tamaño total que ocupa en memoria 
(la longitud de la cadena "STRING OF CHARACTERS" que tiene 21 caracteres, más 1 byte adicional para el carácter nulo de fin de cadena \0)
*/