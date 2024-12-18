#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Para usleep
#include <time.h>   // Para srand y rand

#define NJUGADORES 4
#define MAXREBOTE 70
#define TOPEPUNTOS 10

// Definimos la estructura jugador
struct jugador {
    int dorsal;       // Dorsal del jugador
    int eficTiro;     // Eficiencia en el tiro
    int eficRebote;   // Eficiencia en el rebote
    int puntos;       // Puntos conseguidos durante el entrenamiento
};

// Definimos el estado del balón
enum estadoBalon_t {POSESION, ENAIRE};
enum estadoBalon_t balon; // Estado actual del balón

// Declaramos la variable global dificultadRebote
long int dificultadRebote = 0;

// Inicializamos los jugadores
struct jugador jugadores[NJUGADORES] = {
    {0, 30, MAXREBOTE, 0},  // Jugador 0
    {1, 80, 20, 0},          // Jugador 1
    {2, 40, MAXREBOTE, 0},  // Jugador 2
    {3, 50, 50, 0}           // Jugador 3
};

pthread_mutex_t mutexBalon = PTHREAD_MUTEX_INITIALIZER;

// Funciones
void correr() {
    usleep(100); // Simula el tiempo de correr
}

void tirar(struct jugador* j) {
    pthread_mutex_lock(&mutexBalon);

    // Simula un tiro
    printf("Jugador %d tira al aro.\n", j->dorsal);
    balon = ENAIRE;
    int difTiro = rand() % 100;

    if (difTiro < j->eficTiro) {
        // Canasta
        j->puntos += 2;
        printf("Jugador %d anota una canasta! Puntos: %d\n", j->dorsal, j->puntos);
    } else {
        // Falla el tiro
        printf("Jugador %d falla el tiro.\n", j->dorsal);
    }

    // Generar la dificultad del rebote
    dificultadRebote = rand() % MAXREBOTE;
    printf("Dificultad del rebote: %ld\n", dificultadRebote);

    pthread_mutex_unlock(&mutexBalon);

    // Comprobar si se alcanzó el tope de puntos
    if (j->puntos >= TOPEPUNTOS) {
        printf("Jugador %d ha alcanzado el tope de puntos! Fin del entrenamiento.\n", j->dorsal);
        exit(0);
    }
}

void rebotear(struct jugador* j) {
    while (1) {
        pthread_mutex_lock(&mutexBalon);

        if (balon == ENAIRE && dificultadRebote < j->eficRebote) {
            // Consigue el rebote
            balon = POSESION;
            printf("Jugador %d consigue el rebote.\n", j->dorsal);

            pthread_mutex_unlock(&mutexBalon);
            correr();
            return;
        }

        pthread_mutex_unlock(&mutexBalon);
        usleep(10); // Esperar antes de intentar de nuevo
    }
}

// Función que ejecuta cada hilo (jugador)
void* jugadorInit(void* arg) {
    struct jugador* j = (struct jugador*)arg;

    // Inicializar características del jugador
    printf("Jugador %d: Eficiencia en tiro = %d, Eficiencia en rebote = %d\n",
           j->dorsal, j->eficTiro, j->eficRebote);

    while (1) {
        rebotear(j);
        correr();
        tirar(j);
        correr();
    }

    return NULL;
}

int main() {
    srand(time(NULL)); // Inicializar generador de números aleatorios

    pthread_t hilos[NJUGADORES]; // Array de hilos

    // Crear hilos para cada jugador
    for (int i = 0; i < NJUGADORES; i++) {
        if (pthread_create(&hilos[i], NULL, jugadorInit, (void*)&jugadores[i]) != 0) {
            perror("Error al crear el hilo");
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NJUGADORES; i++) {
        pthread_join(hilos[i], NULL);
    }

    return 0;
}
