#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "./include/treinador.h"
#include "./include/creche.h"


// variaveis da situação
#define NUM_TREINADORES   3
#define NUM_MONSTROS      1
#define NUM_CRECHES       NUM_MONSTROS

// define as características das creches e dos monstros
Creche creches[NUM_CRECHES];
int treinadoresPorCreche[NUM_CRECHES] = { 2 };
int monstrosPorCreche[NUM_CRECHES]    = { 4 };
int timeToMature[NUM_MONSTROS]        = { 3 };

int main() {
  
  pthread_t treinadores[5];

  // inicializa as creches
  for (int i = 0; i < NUM_CRECHES; i++)
    creches[i] = newCreche(treinadoresPorCreche[i], monstrosPorCreche[i]);

  
  for (long int i = 0; i < NUM_TREINADORES; i++)
    pthread_create(&treinadores[i], NULL, treinador, (void*) i);

  for (int i = 0; i < NUM_TREINADORES; i++)
    pthread_join(treinadores[i], NULL);

  return 0;
}