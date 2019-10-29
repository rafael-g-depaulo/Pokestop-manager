#ifndef __CRECHE__
#define __CRECHE__

#include <pthread.h>
#include <semaphore.h>

typedef struct Creche {
  sem_t treinadores;
  sem_t monstros;

} Creche;

Creche newCreche(int numMaxTreinadores, int numMaxMonstros);

#endif /* __CRECHE__ */