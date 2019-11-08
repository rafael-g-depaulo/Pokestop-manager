#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/treinador.h"
#include "include/creche.h"
#include "include/problem_params.h"

// define as características das creches e dos monstros
Creche creches[NUM_CRECHES];
int treinadoresPorCreche[NUM_CRECHES]   = { 2, 2 };
int quantMonstrosPorTipo[NUM_MONSTROS]  = { 4, 7 };
int monstrosPorCreche[NUM_CRECHES]      = { 4, 6 };
int timeToMature[NUM_MONSTROS]          = { 3, 9 };

int main() {
  
  pthread_t treinadores[NUM_TREINADORES];
  Treinador treinadoresFicha[NUM_TREINADORES];

  // seeda o rand()
  srand(time(NULL));

  // inicializa as creches
  for (int i = 0; i < NUM_CRECHES; i++)
    creches[i] = newCreche(treinadoresPorCreche[i], monstrosPorCreche[i], timeToMature[i]);
  
  // inicializa a ficha de cada treinador e as threads deles
  for (long int i = 0; i < NUM_TREINADORES; i++) {
    treinadoresFicha[i] = newTreinador(i, creches, quantMonstrosPorTipo);
    pthread_create(&treinadores[i], NULL, treinador, (void*) &treinadoresFicha[i]);
  }

  for (int i = 0; i < NUM_TREINADORES; i++) {
    pthread_join(treinadores[i], NULL);
    treinadorFree(&treinadoresFicha[i]);
  }

  return 0;
}