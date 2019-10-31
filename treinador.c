#include <stdio.h>
#include "./include/treinador.h"
#include "./include/problem_params.h"

Treinador newTreinador(int id, Creche creches[]) {
  Treinador trainer = {
    id,
    NULL,
    creches
  };

  return trainer;
}

void* treinador(void* _ficha) {
  Treinador * ficha = (Treinador *) _ficha;

  printf("eu sou o treinador #%d\n", ficha->id);
  printf("eu tenho %d monstros do tipo %d\n", ficha->monstros[0], 0);

  return NULL;
}