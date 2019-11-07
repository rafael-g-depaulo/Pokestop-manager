#include <time.h>
#include <stdio.h>
#include <stdarg.h>
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
        sem_getvalue(&creche->monstros, &vagas);
        printf("Trainer #%d: tem %d vagas\n", ficha->id, vagas);
      // TEM QUE CHECAR SE O SEMAFORO TEM A QUANTIDADE CERTA DE VAGAS, NÃO SE ELE TEM 1 DISPONÍVEL
      if (vagas >= mQnt) {
        printf("Trainer #%d: tem vagas o suficiente! (tem %d vagas)\n", ficha->id, vagas);
        // pegar o # de semáforos que eu preciso
        struct timespec tempoWait;
        if (clock_gettime(CLOCK_REALTIME, &tempoWait) == -1) {
          printf("thread %d explodiu\n", ficha->id);
          return NULL;
        }
        clock_gettime(CLOCK_REALTIME, &tempoWait);
        tempoWait.tv_sec += creche->timeToMature;
        for (int i = 0, errNum = 0; i < mQnt; i++) {
          printf("Trainer #%d: vou depositar 1 monstro\n", ficha->id);
          // while ((errNum = sem_timedwait(&creche->monstros, &tempoWait)) == -1)
          //   continue;  
          // sem_timedwait(&creche->monstros, &tempoWait);
          sem_wait(&creche->monstros);
          printf("Trainer #%d: depositei 1 monstro\n", ficha->id);
          totalMonstros--;
          ficha->monstros[mType]--;
          // como timedwait funciona?????????
        }
        pthread_mutex_unlock(&creche->checarVagas);      // libera mutex para outros poderem checar as vagas
        sleep(mQnt);  // demorei 1 segundo para depositar cada monstro

        for (int i = 0, errNum = 0; i < mQnt; i++) {
          printf("Trainer #%d: 1 monstro meu amadureceu\n", ficha->id);
          sem_post(&creche->monstros);
        }

        sem_getvalue(&creche->monstros, &vagas);
        printf("Trainer #%d: agora tem %d vagas\n", ficha->id, vagas);
        
        // // se consegui reservar as vagas, vou depositar os pokemons
        // for (int i = 0; i < mQnt; i++) {
        //   sleep(1);                                     // demora 1s por monstro para depositar
        //   ficha->monstros[mType]--;                     // diminui o total de pokemons daquele tipo
        //   totalMonstros--;                              // diminui o total de pokemons
        // }

        if (totalMonstros > 0)
          sleep(5);                                       // espera um pouco e deposita seus pokemons de novo
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