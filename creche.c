#include "creche.h"

Creche newCreche(int numMaxTreinadores, int numMaxMonstros) {
  Creche creche;
  sem_init(&creche.treinadores, 0, numMaxTreinadores);
  sem_init(&creche.monstros, 0, numMaxMonstros);
  return creche;
}