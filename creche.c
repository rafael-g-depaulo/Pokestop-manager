#include "./include/creche.h"
#include <semaphore.h>
#include <pthread.h>

Creche newCreche(int numMaxTreinadores, int numMaxMonstros, int timeToMature) {
  Creche creche;
  sem_init(&creche.treinadores, 0, numMaxTreinadores);
  sem_init(&creche.monstros, 0, numMaxMonstros);
  creche.timeToMature = timeToMature;
  return creche;
}