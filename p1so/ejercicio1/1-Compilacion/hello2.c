#include <stdio.h>

#define N 5

#define min(x,y) ( (x<y)?x:y )
int a = 7;
int b = 9;
int main() {

 char* cad = "Hello world";
 int i;

 for (i=0;i<N;i++) {
   printf("%s \t a= %d b= %d\n",cad,a,b);
   a++;
   a = min(a,b);
 }
 return 0;
}

/*
1. Compilar el codigo

  -gcc -c hello2.c
  -gcc -o ejercicio1 hello2.o
  -ejecutar:
      ssoo@ssoohost:~/p1so/ejercicio1/1-Compilacion$ ./hello2 
      Hello world 	 a= 7 b= 9
      Hello world 	 a= 8 b= 9
      Hello world 	 a= 9 b= 9
      Hello world 	 a= 9 b= 9
      Hello world 	 a= 9 b= 9

  -Obtener la salida de todas las etapas intermedias:
      gcc --save-temps hello2.c

  -Lo que ha pasado en el archivo a-hello2.i(archivo intermedio):
      la macro #define min(x,y) ( (x<y) ? x:y ) se ha sustituido en la linea 
      16 cuando se llama a min(a,b) por ( (x<y) ? x:y )

  -Gracias a <stdio> se ha podido usar printf() para imprimir por pantalla y otras funciones de E/S.
   <stdio> es esencial para que el codigo se ejecute  compile correctamente.
   Sin esta directiva el programa no funcionaria.
*/