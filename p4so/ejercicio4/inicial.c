#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int fd1, i;
    char buffer[6];

    // Abre el archivo y escribe los cinco ceros iniciales
    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    write(fd1, "00000", 5);

    for (i = 0; i < 10; i++) {
        if (fork() == 0) {
            // Proceso hijo
            sprintf(buffer, "%d", i * 11111);
            lseek(fd1, 1 + (i * 10), SEEK_SET); // Posición del hijo
            write(fd1, buffer, 5);
            close(fd1);
            exit(0);
        } else {
            // Proceso padre
            wait(NULL); // Espera a que el hijo termine
            sprintf(buffer, "%d", i * 10000);
            lseek(fd1, 5 + (i * 10), SEEK_SET); // Posición del padre
            write(fd1, buffer, 5);
        }
    }

    // Lee y muestra el contenido del archivo
    lseek(fd1, 0, SEEK_SET);
    printf("File contents are:\n");
    char c;
    while (read(fd1, &c, 1) > 0)
        printf("%c", c);
    printf("\n");
    close(fd1);
    return 0;
}


/*

#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *status);
se utiliza para hacer que un proceso padre espere a que cualquiera de sus procesos hijos termine

Parámetros
status:
Un puntero a un entero donde se almacena información sobre cómo terminó el proceso hijo.
Si no necesitas esta información, puedes pasar NULL.

Valor de retorno
PID del hijo que cambió de estado si tiene éxito.
-1 si hay un error (por ejemplo, si no hay procesos hijos activos).
Establece errno para describir el error en caso de fallo.

#include <stdio.h>

int sprintf(char *str, const char *format, ...);
se utiliza para escribir datos formateados en una cadena de caracteres en lugar de enviarlos directamente a la salida estándar (como lo haría printf).
Es útil cuando necesitas manipular o almacenar una cadena antes de imprimirla o procesarla.
Parámetros
str:

Un puntero a un búfer donde se almacenará la cadena resultante.
Debe tener suficiente espacio para contener la cadena generada.
format:

Una cadena de formato similar a la utilizada en printf.
Incluye especificadores como %d, %s, %f, etc., para insertar datos en la cadena.
...:

Una lista variable de argumentos que corresponden a los especificadores de formato

#include <unistd.h>

off_t lseek(int fd, off_t offset, int whence);

se utiliza para mover el puntero de lectura/escritura en un archivo abierto. 
Esto es útil para cambiar la posición en un archivo sin necesidad de leer o escribir datos, permitiendo la manipulación avanzada del archivo

Parámetros
fd:

El descriptor de archivo al que se desea mover el puntero de lectura/escritura. Este descriptor se obtiene al abrir el archivo mediante la función open.
offset:

El desplazamiento en bytes desde la posición indicada por whence. El valor puede ser positivo o negativo, dependiendo de la operación que se desea realizar.
whence:

Define la posición desde la cual se realiza el desplazamiento. Puede ser uno de los siguientes valores:
	SEEK_SET: El desplazamiento se realiza desde el inicio del archivo (es decir, offset es el número de bytes desde el principio del archivo).
	SEEK_CUR: El desplazamiento se realiza desde la posición actual del puntero de lectura/escritura (es decir, offset se suma o resta de la posición actual).
	SEEK_END: El desplazamiento se realiza desde el final del archivo (es decir, offset es el número de bytes desde el final del archivo, siendo offset negativo para ir hacia atrás).

CAMBIOS:

Posicionamiento Específico: Cada proceso hijo calcula su posición de escritura como i * 5, 
lo que asegura que cada uno escriba en su propio bloque de 5 caracteres, evitando colisiones.
Eliminación del uso de lseek en el padre: Ya que el padre escribe ceros en la posición inicial y los hijos están manejando sus propias posiciones,
no es necesario que el padre ajuste la posición.
Código de Salida: Cambié exit(0) al final de main a return 0, lo cual es una práctica más común en C.
*/


/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    int fd1,fd2,i,pos;
    char c;
    char buffer[6];

    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    write(fd1, "00000", 5);
    for (i=1; i < 10; i++) {
        pos = lseek(fd1, 0, SEEK_CUR);
        if (fork() == 0) {
            / Child 
            sprintf(buffer, "%d", i*11111);
            lseek(fd1, pos, SEEK_SET);
            write(fd1, buffer, 5);
            close(fd1);
            exit(0);
        } else {
            / Parent 
            lseek(fd1, 5, SEEK_CUR);
        }
    }

	//wait for all childs to finish
    while (wait(NULL) != -1);

    lseek(fd1, 0, SEEK_SET);
    printf("File contents are:\n");
    while (read(fd1, &c, 1) > 0)
        printf("%c", (char) c);
    printf("\n");
    close(fd1);
    exit(0);
}
*/
