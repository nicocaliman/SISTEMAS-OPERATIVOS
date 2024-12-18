#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NJUGADORES 4
#define MAXREBOTE 70

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

// Función que ejecuta cada hilo (jugador)
void* jugadorInit(void* arg) {
    struct jugador* jug = (struct jugador*)arg;

    // Imprimir las características del jugador
    printf("Jugador %d: Eficiencia en tiro = %d, Eficiencia en rebote = %d, Puntos = %d\n",
           jug->dorsal, jug->eficTiro, jug->eficRebote, jug->puntos);

    return NULL;
}

int main() {
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
