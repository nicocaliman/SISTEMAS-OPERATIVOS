#include <stdio.h>
#include <pthread.h>

// Función que ejecutará cada hilo
void* print_thread_number(void* arg) {
    int thread_num = *((int*)arg);
    printf("Hilo número: %d\n", thread_num);
    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_args[10];

    // Crear 10 hilos
    for (int i = 0; i < 10; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, print_thread_number, &thread_args[i]);
    }

    // Esperar a que terminen todos los hilos
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
