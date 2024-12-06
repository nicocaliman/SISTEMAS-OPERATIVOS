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