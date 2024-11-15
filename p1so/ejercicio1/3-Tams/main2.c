#include <stdio.h>


#define ARRAY_SIZE  10
#define ARRAY2_SIZE 7

int a = 7;
unsigned long b = 8;
short c;
char x;
char* pc;

int array1[ARRAY_SIZE];
int array2[ARRAY2_SIZE]; //int array2[a];

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
ssoo@ssoohost:~/p1so/ejercicio1/3-Tams$ ./main2
Address of a: 0x5646e4696010 Size: 4 
Address of b: 0x5646e4696018 Size: 8 
Address of c: 0x5646e4696060 Size: 2 
Address of x: 0x5646e4696062 Size: 1 
Address of pc: 0x5646e4696068 Address pointed by pc: 0x5646e4696062 Size: 8 
Address of array: 0x5646e4696080 Addres of elemnt 0: 0x5646e4696080 Size of the array: 40 
Address of str1: 0x5646e4696038 Address pointed by it: 0x5646e4694008 Size: 8 
Address of str2: 0x5646e4696020 Address pointed by it: 0x5646e4696020 Size: 21 

	-Tenemos un problema de compilacion, pues el tamanio del array de enteros no es constante (int array2[a];)
	El tamanio de los arrays ha de ser CONSTANTE.

	- %lu = unsigned long

	-pc apunta a la direccion de memoria de la variable tipo char 'x': 

	Address of x: 0x5646e4696062
	Address pointed by pc: 0x5646e4696062

	Los tamanios no son iguales, pues size of x: 1 y size of pc: 8
	
	-El valor del tamanio del array NO coincide con el numero de elementos del array. Esto es porque el array al ser entero,
	cada valor entero que se almacena en cada celda ocupa 4 Bytes.

	Por tanto, 4*10 = 40 Bytes de tamanio total ocupa el array1[].
	sizeof() devuelve TAMANIO_ARRAY*sizeof(int)


	- str1 y str2 apunta a distintasa direcciones de memoria, por tanto, no coinciden sus direcciones.

	-los tamanios de str1 y str2 son diferentes, pues str1 es de tipo (puntero char) mientras que str2 es un array de tipo char.
	Resulta que el tamanio de str1 es de 8 bytes pues estamos en un sistema de 64 bits (8*8 bits = 64). Sin embargo, el tamanio de str2 
	es el numero de caracteres (1 char = 1 byte) + byte terminador ('\0').

*/