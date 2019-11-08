#ifndef __CRECHE__
#define __CRECHE__

#include <pthread.h>
#include <semaphore.h>

typedef struct Creche {
  sem_t treinadores;
  sem_t monstros;
  pthread_mutex_t checarVagas;
  int timeToMature;
  int vagasReservas;
} Creche;

Creche newCreche(int numMaxTreinadores, int numMaxMonstros, int timeToMature);

#endif /* __CRECHE__ */