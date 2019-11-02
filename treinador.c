#include <stdio.h>
#include <stdlib.h>
#include "./include/treinador.h"
#include "./include/problem_params.h"

Treinador newTreinador(int id, Creche* creches, int* monstros) {
  Treinador trainer = {
    .id = id + 1
  };

  trainer.monstros = malloc(NUM_MONSTROS * sizeof(int));
  trainer.creches = malloc(NUM_CRECHES * sizeof(int));

  for (int i = 0; i < NUM_MONSTROS; i++)
    trainer.monstros[i] = monstros[i];
  for (int i = 0; i < NUM_CRECHES; i++)
    trainer.creches[i] = &creches[i];

  return trainer;
}

void treinadorFree(Treinador* treinador) {
  free(treinador->creches);
  free(treinador->monstros);
}


void* treinador(void* _ficha) {
  Treinador * ficha = (Treinador *) _ficha;
  int totalMonstros = 0;
  for (int i = 0; i < NUM_MONSTROS; i++)
    totalMonstros += ficha->monstros[i];

  // início do loop
  while (totalMonstros > 0) {
    // escolhe quantos montypestros e de qual tipo depositar
    int mType = getMonsterType(ficha->monstros);
    int mQnt  = rand() % 5 + 1;
    mQnt = mQnt > ficha->monstros[mType] ? ficha->monstros[mType] : mQnt;

    printf("A quantidade de monstros é %d\n", totalMonstros);
    printf("Eu sou o treinador #%d, e pretendo %d monstros do tipo %d\n", ficha->id, mQnt, mType);
    
    // temp para não ser loop infinito
    totalMonstros -= mQnt;
  }

  printf("eu sou o treinador #%d, e terminei a minha jornada!\n", ficha->id);
  return NULL;
}

// escolhe aleatóriamente qual tipo de monstro depositar
int getMonsterType(int monstros[]) {
  int randIndex;
  do {
    randIndex = rand() % NUM_MONSTROS;
  } while (monstros[randIndex] <= 0);
  printf("monster type: %d\n", randIndex);
  return randIndex;
}

// escolhe aleatoriamente a quantidade de monstros pra depositar
// (sem passar do limite de monstros que o treinador tem)
int getMonsterQnt(int monstros) {
  int randQnt = rand() % 5 + 1;
  randQnt = randQnt > monstros ? monstros : randQnt;
  printf("A quantidade de monstros é %d\n", monstros);
  return randQnt;
}