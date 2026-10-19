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
Para compilar el codigo 

  1. gcc -c hello2.c 

  2. gcc -o hello2 hello2.c

  3. ./hello2

*/

/*
  gcc --save-temps hello2.c
*/

/*
En su lugar, el código utiliza un operador ternario para realizar esa operación:

  a = ( (a<b)?a:b ); 

  Es basicamente la definicion de la macro de min(x,y)
*/

/*
La directiva #include <stdio.h> ha tenido como efecto insertar y expandir el contenido completo del archivo de cabecera estándar de entrada
 y salida (stdio.h) (junto con sus dependencias) directamente en el archivo antes de que pase por el compilador.
*/