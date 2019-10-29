#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "treinador.h"
#include "creche.h"

// variaveis da situação
#define NUM_TREINADORES   3
#define NUM_MONSTROS      1
#define NUM_CRECHES       NUM_MONSTROS

// define as características das creches e dos monstros
Creche creches[NUM_CRECHES];
int treinadoresPorCreche[NUM_CRECHES] = { 2 };
int monstrosPorCreche[NUM_CRECHES]    = { 4 };
int timeToMature[NUM_MONSTROS]        = { 3 };

int main () {

  // inicializa as creches
  for (int i = 0; i < NUM_CRECHES; i++)
    creches[i] = newCreche(treinadoresPorCreche[i], monstrosPorCreche[i]);


  pthread_t threads[5];
  
  for (long int i = 0; i < 5; i++)
    pthread_create(&threads[i], NULL, treinador, (void*) i);

  for (int i = 0; i < 5; i++)
    pthread_join(threads[i], NULL);

  return 0;
}