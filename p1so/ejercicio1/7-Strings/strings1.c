#include <stdio.h>
#include <string.h>

int main()
{
	char msg[10]; /* array of 10 chars */
	char *p;      /* pointer to a char */
	char msg2[28] = "Hello";  /* msg2 = 'H' 'e' 'l' 'l' 'o' '\0' */

	p   = "Bonjour";
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	p = msg2;
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("dir de msg: %p, dir de p: %p, dir de msg2: %p\n", msg, p, msg2);

	//	p[0] = 'H', p[1] = 'i', p[2] = '\0';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("msg len: %lu p len %lu msg2 len %lu\n", strlen(msg), strlen(p), strlen(msg2));
	printf("msg size: %lu p size %lu msg2 size %lu\n", sizeof(msg), sizeof(p), sizeof(msg2));

	msg[0] = 'B', msg[1] = 'y';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
}

/*
ssoo@ssoohost:~/p1so/ejercicio1/7-Strings$ ./strings1 
msg: , p: Bonjour, msg2: Hello
dir de msg: 0x7ffed3d5e106, dir de p: 0x55aee3e92008, dir de msg2: 0x7ffed3d5e110
msg: , p: Hello, msg2: Hello
dir de msg: 0x7ffed3d5e106, dir de p: 0x7ffed3d5e110, dir de msg2: 0x7ffed3d5e110
msg: , p: Hi, msg2: Hi
msg len: 0 p len 2 msg2 len 2
msg size: 10 p size 8 msg2 size 28
msg: By, p: Hi, msg2: Hi

	-Linea que da error: p[0] = 'H', p[1] = 'i', p[2] = '\0';
	-El error se manifiesta en ejecucion pues la manipulacion de una cadena constante induce al error.

	-La 'B' de Bonjour se encuentra en la direccion p: 0x55aee3e92008 y la letra 'j' se encuentra en 0x55aee3e9200B

	-Para mantener la direccion de la cadena Bonjour podemos hacer uso de otro puntero auxiliar *p_aux.

	-La longitud de p y msg2 es 2 porque tras la línea comentada, estás modificando la cadena apuntada por p, que es msg2.
	 La asignación p[0] = 'H', p[1] = 'i', p[2] = '\0'; asigna 'H' y 'i', terminando la cadena con '\0'.
	 Así, tanto p como msg2 ahora son "Hi", que tiene longitud 2.

	-strlen() devuelve la longitud de la cadena, es decir, el número de caracteres hasta el carácter nulo ('\0'). 
	En cambio, sizeof() devuelve el tamaño total en bytes que ocupa la variable. 
*/