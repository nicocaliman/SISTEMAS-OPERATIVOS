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

	p[0] = 'H', p[1] = 'i', p[2] = '\0';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
	printf("msg len: %lu p len %lu msg2 len %lu\n", strlen(msg), strlen(p), strlen(msg2));
	printf("msg size: %lu p size %lu msg2 size %lu\n", sizeof(msg), sizeof(p), sizeof(msg2));

	msg[0] = 'B', msg[1] = 'y';
	printf("msg: %s, p: %s, msg2: %s\n", msg, p, msg2);
}

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/7-Strings$ ./strings1 
msg: , p: Bonjour, msg2: Hello
dir de msg: 0x7fffdc75a406, dir de p: 0x5a7ba62f0008, dir de msg2: 0x7fffdc75a410
msg: , p: Hello, msg2: Hello
dir de msg: 0x7fffdc75a406, dir de p: 0x7fffdc75a410, dir de msg2: 0x7fffdc75a410
msg: , p: Hi, msg2: Hi
msg len: 0 p len 2 msg2 len 2
msg size: 10 p size 8 msg2 size 28
msg: By8�u��, p: Hi, msg2: Hi

El error se manifiesta en ejecución (Segmentation Fault / fallo de segmentación).

Se produce al intentar escribir o modificar el contenido de la cadena "Bonjour" a través del puntero p (por ejemplo, con p[0] = 'b' o strcpy(p, ...)). Las cadenas literales entre comillas se almacenan en la sección de memoria de solo lectura (.rodata). El compilador lo permite, pero en tiempo de ejecución el sistema operativo bloquea el intento de escritura en esa memoria protegida.

Solución: Se debe comentar la línea de código donde se intenta escribir directamente en p mientras este apunta a la constante "Bonjour"

La direccion de B esta en 0x5a7ba62f0008 y la de j estara en 0x5a7ba62f0008 + (3 bytes) = 0x5a7ba62f000B

No se puede recuperar. Al ejecutar p = msg2, el puntero p sobrescribe su dirección (0x5a7ba62f0008) por la de msg2 (0x7fffdc75a410). Como no se guardó previamente esa dirección en otra variable puntero (por ejemplo, char *p_backup = p;), 
el programa pierde la única referencia a la ubicación de "Bonjour" en memoria.


Como p apunta a la direccion de msg2 sobreescribe el contenido de msg2 ('hi\0') por eso la longitud de la cadena es 2.
La función strlen() recorre la memoria y cuenta únicamente los caracteres presentes antes de encontrar el terminador nulo '\0'. Por ello, aunque se escribieron 3 bytes en memoria, la longitud de la cadena es 2.

strlen() calcula la longitud de la cadena sin contar el caracter nulo '\0'. Por el contrario sizeof() devuelve el tamanio que ocupa el tipo de la variable (int, double, float, char...)



*/