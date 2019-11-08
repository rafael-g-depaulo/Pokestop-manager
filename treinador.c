#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "include/creche.h"
#include "include/treinador.h"
#include "include/problem_params.h"
#include "include/monstro.h"

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

  int* monstrosInicio = malloc(NUM_MONSTROS * sizeof(int));

  // separar as threads pra cada monstro
  pthread_t** monstros = malloc(NUM_MONSTROS * sizeof(pthread_t*));
  for (int tipoMonstros = 0; tipoMonstros < NUM_MONSTROS; tipoMonstros++) {

    monstros[tipoMonstros] = malloc(ficha->monstros[tipoMonstros] * sizeof(pthread_t));
  }

  // início do loop
  while (totalMonstros > 0) {
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
        printf("Trainer #%d: ------------------------------------\n", ficha->id);
        pthread_mutex_lock(&creche->checarVagas);
        printf("Trainer #%d: consegui o lock para checar vagas\n", ficha->id);
        vagas = creche->vagasReservas;
        printf("Trainer #%d: tem %d vagas\n", ficha->id, vagas);
      // TEM QUE CHECAR SE O SEMAFORO TEM A QUANTIDADE CERTA DE VAGAS, NÃO SE ELE TEM 1 DISPONÍVEL
      if (vagas >= mQnt) {
        creche->vagasReservas -= mQnt;
        pthread_mutex_unlock(&creche->checarVagas);
        
        printf("Trainer #%d: tem vagas o suficiente! (tem %d vagas)\n", ficha->id, vagas);
        for (int i = 0, errNum = 0; i < mQnt; i++) {
          // deposita o monstro 
          sleep(1);
          ficha->monstros[mType]--;
          totalMonstros--;

          pthread_create(&monstros[mType][ficha->monstros[mType]], NULL, monstro, (void*) creche);
          printf("Trainer #%d: depositei 1 monstro\n", ficha->id);
        }
        
        // demorei 1 segundo para depositar cada monstro
        sleep(mQnt);

        for (int i = 0, errNum = 0; i < mQnt; i++) {
          printf("Trainer #%d: 1 monstro meu amadureceu\n", ficha->id);
          sem_post(&creche->monstros);
        }

        sem_getvalue(&creche->monstros, &vagas);
        printf("Trainer #%d: agora tem %d vagas\n", ficha->id, vagas);

        // espera um pouco e deposita seus pokemons de novo
        if (totalMonstros > 0)
          sleep(5);
        
      } else {
        pthread_mutex_unlock(&creche->checarVagas);      // libera mutex para outros poderem checar as vagas

        printf("Trainer #%d: não tinham vagas o suficiente, vou sair da recepção agora\n", ficha->id);
        //  se eu não consegui vagas, repensar a vida e tentar de novo
        sleep(2);
      }
      printf("Trainer #%d: parei de usar meu espaço na recepção da creche %d\n", ficha->id, mType);
      sem_post(&creche->treinadores);   // libera o semáforo
      // sem_post(&creche->treinadores);   // libera o semáforo
    } else {
      // se não consegui um espaço, vou esperar um pouco e tentar depois
      printf("Trainer #%d: NÃO CONSEGUI espaço na creche %d\n", ficha->id, mType);
      sleep(1);
    }
  }

  for (int tipoMonstros = 0; tipoMonstros < NUM_MONSTROS; tipoMonstros++) {
    for (int i = 0; i < monstrosInicio[tipoMonstros]; i++)
      pthread_join(monstros[tipoMonstros][i], NULL);
    free(monstros[tipoMonstros]);
  }
  free(monstros);

  printf("Trainer #%d: terminei a minha jornada!\n", ficha->id);
  pthread_exit(NULL);
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