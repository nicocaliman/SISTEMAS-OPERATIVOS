#include <math.h>

double pi = M_PI;

double sinDegrees(double x) {
	double rad = (x*M_PI/180);
	return sin(rad);
}

double cosDegrees(double x) {
	double rad = (x*M_PI/180);
	return cos(rad);
}

/*
	touch  -  Update  the  access and modification times of each FILE to the current time.
*/

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ touch auxiliar.c
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ make
gcc -Wall -g -c -o auxiliar.o auxiliar.c
gcc -Wall -g -o prueba auxiliar.o init.o -lm

Se ha eliminado una linea gcc del compilador tras hacer touch

make solo recompilará el archivo modificado (auxiliar.o) y volverá a enlazar el proyecto. No volverá a compilar init.o.

ya que make compara las fechas de modificación (timestamps) de los ficheros fuente y los objetos. Como init.c no ha cambiado, 
init.o se considera actualizado, lo que ahorra tiempo de compilación en proyectos grandes.
*/

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ make clean
rm *.o prueba
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ ls
auxiliar.c  auxiliar.h  init.c  makefile

Ejecuta el comando -rm *.o prueba, lo que borra todos los archivos objeto y el ejecutable prueba, dejando el directorio limpio.

La directiva .PHONY le avisa a make que clean es un nombre simbólico (una acción) y no un archivo real.


vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ make
gcc -Wall -g -c -o auxiliar.o auxiliar.c
gcc -Wall -g -c -o init.o init.c
gcc -Wall -g -o prueba auxiliar.o init.o -lm
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ ls
auxiliar.c  auxiliar.h  auxiliar.o  init.c  init.o  makefile  prueba

vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ make
gcc -Wall -g -c -o auxiliar.o auxiliar.c
gcc -Wall -g -c -o init.o init.c
gcc -Wall -g -o prueba auxiliar.o init.o -lm
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ ls
auxiliar.c  auxiliar.h  auxiliar.o  init.c  init.o  makefile  prueba


Si creas un archivo llamado clean sin .PHONY: Al ejecutar make clean, make verá que ya existe un archivo llamado clean en el directorio.
Como no hay dependencias nuevas, pensará que el objetivo ya está actualizado y no ejecutará la orden de borrado, mostrando un mensaje como "clean is up to date".
*/

/*
vboxuser@SO:~/SO/practica1/ficheros_p1/ejercicio1/2-Make$ make
gcc -Wall -g -c -o auxiliar.o auxiliar.c
gcc -Wall -g -c -o init.o init.c
gcc -Wall -g -o prueba auxiliar.o init.o 
/usr/bin/x86_64-linux-gnu-ld.bfd: auxiliar.o: in function `sinDegrees':
/home/vboxuser/SO/practica1/ficheros_p1/ejercicio1/2-Make/auxiliar.c:7:(.text+0x3d): undefined reference to `sin'
/usr/bin/x86_64-linux-gnu-ld.bfd: auxiliar.o: in function `cosDegrees':
/home/vboxuser/SO/practica1/ficheros_p1/ejercicio1/2-Make/auxiliar.c:12:(.text+0x80): undefined reference to `cos'
collect2: error: ld returned 1 exit status
make: *** [makefile:9: all] Error 1


Como el código utiliza funciones matemáticas (sin(), cos()), fallará.

Da problemas en la etapa de enlace (linking). El compilador generará los archivos objeto sin problemas, pero al intentar unirlos todos en el ejecutable prueba,
gcc mostrará un error de tipo "undefined reference to [función matemática]" porque no sabrá dónde encontrar el código fuente de esas operaciones matemáticas.
*/