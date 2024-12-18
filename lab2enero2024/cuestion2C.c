#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_NAME "output.txt"

pthread_mutex_t file_mutex;

// Función que ejecutará cada hilo
void* write_to_file_sync(void* arg) {
    int thread_num = *((int*)arg);

    // Generar los datos a escribir
    char buffer[6];
    for (int i = 0; i < 5; i++) {
        buffer[i] = '0' + thread_num;
    }
    buffer[5] = '\0';

    // Bloquear el acceso al fichero
    pthread_mutex_lock(&file_mutex);

    int fd = open(FILE_NAME, O_WRONLY);
    if (fd < 0) {
        perror("Error al abrir el fichero");
        pthread_mutex_unlock(&file_mutex);
        pthread_exit(NULL);
    }

    // Calcular la posición correspondiente
    off_t pos = thread_num * 5;
    lseek(fd, pos, SEEK_SET);

    // Escribir en el fichero
    write(fd, buffer, 5);
    close(fd);

    // Desbloquear el acceso
    pthread_mutex_unlock(&file_mutex);

    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_args[10];

    // Inicializar el mutex
    pthread_mutex_init(&file_mutex, NULL);

    // Crear el fichero y truncarlo
    int fd = open(FILE_NAME, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Error al crear el fichero");
        return 1;
    }
    close(fd);

    // Crear 10 hilos
    for (int i = 0; i < 10; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, write_to_file_sync, &thread_args[i]);
    }

    // Esperar a que terminen todos los hilos
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Leer y mostrar el contenido final del fichero
    fd = open(FILE_NAME, O_RDONLY);
    if (fd < 0) {
        perror("Error al abrir el fichero para lectura");
        return 1;
    }

    printf("Contenido final del fichero:\n");
    char c;
    while (read(fd, &c, 1) > 0) {
        putchar(c);
    }
    printf("\n");
    close(fd);

    // Destruir el mutex
    pthread_mutex_destroy(&file_mutex);

    return 0;
}