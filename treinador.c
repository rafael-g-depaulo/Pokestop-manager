#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

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
    printf("Trainer #%d: tenho %d pokemons\n", ficha->id, totalMonstros);

    // escolhe quantos monstros e de qual tipo depositar
    int mType = getMonsterType(ficha->monstros);
    int mQnt  = getMonsterQnt(ficha->monstros[mType]);
    // mQnt = mQnt > ficha->monstros[mType] ? ficha->monstros[mType] : mQnt;

    printf("Trainer #%d: pretendo depositar %d pokemons do tipo %d\n", ficha->id, mQnt, mType);

    // tenta pegar um espaço na creche do tipo de monstro escolhido
    if (sem_trywait(ficha->creches[mType]) == 0) {
      // se consegui um espaço, vou depositar meus pokemons
      printf("Trainer #%d: consegui pegar um espaço na creche %d\n", ficha->id, mType);
      sleep(2);
      printf("Trainer #%d: parei de usar meu espaço na creche %d\n", ficha->id, mType);
      sem_post(ficha->creches[mType]);
      totalMonstros -= mQnt;
      printf("Trainer #%d: depositei %d pokemon na creche %d\n", ficha->id, mQnt, mType);
    } else {
      // se não consegui um espaço, vou esperar um pouco e tentar depois
      printf("Trainer #%d: NÃO CONSEGUI espaço na creche %d\n", ficha->id, mType);
      sleep(1);
    }
  }

  printf("Trainer #%d: terminei a minha jornada!\n", ficha->id);
  return NULL;
}

// escolhe aleatóriamente qual tipo de monstro depositar
int getMonsterType(int monstros[]) {
  int randIndex;
  do {
    randIndex = rand() % NUM_MONSTROS;
  } while (monstros[randIndex] <= 0);
  return randIndex;
}

// escolhe aleatoriamente a quantidade de monstros pra depositar
// (sem passar do limite de monstros que o treinador tem)
int getMonsterQnt(int monstros) {
  int randQnt = rand() % 5 + 1;
  randQnt = randQnt > monstros ? monstros : randQnt;
  return randQnt;
}