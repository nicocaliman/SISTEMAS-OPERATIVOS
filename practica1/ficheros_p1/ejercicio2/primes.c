/**
 *  This program calculates the sum of the first n prime
 *  numbers. Optionally, it allows the user to provide as argument the
 *  value of n, which is 10 by default.
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/**
 * This function takes an array of integers and returns the sum of its n elements.
 */
int sum(int *arr, int n);

/**
 * This function fills an array with the first n prime numbers.
 */
void compute_primes(int* result, int n);

/**
 * This function returns 1 if the integer provided is a prime, 0 otherwise.
 */
int is_prime(int x);

int main(int argc, char **argv) {

  int n = 10; // by default the first 10 primes
  
  //modificado (argc = 2)
  if(argc == 2) {
    //atoi(argv[2]);
    n=atoi(argv[1]);
  }
  int* primes = (int*)malloc(n*sizeof(int));
  compute_primes(primes, n);

  int s = sum(primes, n);
  printf("The sum of the first %d primes is %d\n", n, s);

  free(primes);
  return 0;
}

int sum(int *arr, int n) {
  int i;
  //int total;
  int total = 0;

  for(i=0; i<n; i++) {
    //total =+ arr[i];
    total += arr[i];
  }
  return total;
}

void compute_primes(int* result, int n) {
  int i = 0;
  int x = 2;
  while(i < n) {
    if(is_prime(x)) {
      result[i] = x;
      i++;
      //x += 2;
    }
    x++;
  }
  return;
}

int is_prime(int x) {
  /*if(x % 2 == 0) {
    return 0;
  }*/
if(x<2){
  return 0;
}
else if(x == 2){
  return 1;
}
else if(x%2==0){
  return 0;
}

  for(int i=3; i<x; i+=2) {
    if(x % i == 0) {
      return 0;
    }
  }
  return 1;
}

/*
Errores identificados

    Error en el procesamiento de argumentos (main)

        Problema: if(argc = 2) realiza una asignación en vez de una comparación (==). Además, argv[2] accede a un argumento inexistente (el primer argumento pasado es argv[1]), y el retorno de atoi() no se asigna a n.

        Comportamiento: Provoca un fallo de segmentación (Segmentation Fault) si se ejecuta sin parámetros.

    Error al evaluar si el número 2 es primo (is_prime)

        Problema: La condición if(x % 2 == 0) return 0; devuelve 0 cuando x = 2, descartando el 2 como número primo (el único primo par).

    Bucle infinito e incremento incorrecto (compute_primes)

        Problema: x += 2 está dentro del if(is_prime(x)). Si x no es primo, x nunca se incrementa y el bucle while se ejecuta infinitamente. Además, avanzar de 2 en 2 desde un número par se saltaría todos los números impares.

    Acumulación incorrecta y variable sin inicializar (sum)

        Problema: int total; no se inicializa a 0 (contiene basura de memoria). Además, se escribió total =+ arr[i] (asigna el valor positivo) en lugar de total += arr[i] (suma acumulativa).
        */