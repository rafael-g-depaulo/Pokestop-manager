#include "./include/creche.h"
#include <semaphore.h>
#include <pthread.h>

Creche newCreche(int numMaxTreinadores, int numMaxMonstros, int timeToMature) {
  Creche creche;
  sem_init(&creche.treinadores, 0, numMaxTreinadores);
  sem_init(&creche.monstros, 0, numMaxMonstros);
  pthread_mutex_init(&creche.checarVagas, NULL);
  creche.timeToMature = timeToMature;
  return creche;
}