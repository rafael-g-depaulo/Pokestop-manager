#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
  while (totalMonstros-- > 0) {
    printf("Trainer #%d: tenho %d pokemons\n", ficha->id, totalMonstros);

    // escolhe quantos monstros e de qual tipo depositar
    int mType = getMonsterType(ficha->monstros);
    int mQnt  = getMonsterQnt(ficha->monstros[mType]);
    
    // variavel temporárias pra ficar mais simples
    Creche* creche = ficha->creches[mType];

    printf("Trainer #%d: pretendo depositar %d pokemons do tipo %d\n", ficha->id, mQnt, mType);

    // tenta pegar um espaço na creche do tipo de monstro escolhido
    if (sem_trywait(&creche->treinadores) == 0) {
      // se consegui um espaço, vou depositar meus pokemons
      printf("Trainer #%d: consegui pegar um espaço na creche %d\n", ficha->id, mType);
      
      // tenta reservar vagas para os pokemons que eu quero depositar

      // TEM QUE CHECAR SE O SEMAFORO TEM A QUANTIDADE CERTA DE VAGAS, NÃO SE ELE TEM 1 DISPONÍVEL
        int vagas = 0;
        pthread_mutex_lock(&creche->checarVagas);
        sem_getvalue(&creche->monstros, &vagas);
      // TEM QUE CHECAR SE O SEMAFORO TEM A QUANTIDADE CERTA DE VAGAS, NÃO SE ELE TEM 1 DISPONÍVEL
      if (vagas >= mQnt) {
        // pegar o # de semáforos que eu preciso
        pthread_mutex_unlock(&creche->checarVagas);      // libera mutex para outros poderem checar as vagas
        for (int i = 0; i < mQnt; i++) {
          struct timespec tempoWait;
          // sleep(1);
          // clock_gettime(CLOCK_REALTIME, &tempoWait);
          // tempoWait.tv_sec += creche->timeToMature;
          // sem_timedwait(&creche->monstros, &tempoWait);
          // como timedwait funciona?????????
        }

        // se consegui reservar as vagas, vou depositar os pokemons
        for (int i = 0; i < mQnt; i++) {
          sleep(1);                                     // demora 1s por monstro para depositar
          ficha->monstros[mType]--;                     // diminui o total de pokemons daquele tipo
          totalMonstros--;                              // diminui o total de pokemons
        }
        // sleep(5);                                       // espera um pouco e deposita seus pokemons de novo
      } else {
        pthread_mutex_unlock(&creche->checarVagas);      // libera mutex para outros poderem checar as vagas
        //  se eu não consegui vagas, repensar a vida e tentar de novo
        sleep(2);
      }
      sem_post(&creche->treinadores);   // libera o semáforo
      printf("Trainer #%d: parei de usar meu espaço na creche %d\n", ficha->id, mType);
      // sem_post(&creche->treinadores);   // libera o semáforo
    }

    // se não consegui um espaço, vou esperar um pouco e tentar depois
    printf("Trainer #%d: NÃO CONSEGUI espaço na creche %d\n", ficha->id, mType);
    sleep(1);
  }

  printf("Trainer #%d: terminei a minha jornada!\n", ficha->id);
  pthread_exit(NULL);
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