#include "include/creche.h"
#include "include/monstro.h"

#include <pthread.h>

void* monstro(void* args) {
  Creche* creche = (Creche*) args;

  // pega o semáforo
  sem_wait(&creche->monstros);

  // fica na creche até amadurecer
  sleep(creche->timeToMature);

  // libera a vaga na creche
  sem_post(&creche->monstros);

  // libera a reserva (é diferente da vaga na creche)
  pthread_mutex_lock(&creche->checarVagas);
  creche->vagasReservas++;
  pthread_mutex_unlock(&creche->checarVagas);

  // fecha a thread
  pthread_exit(NULL);
}