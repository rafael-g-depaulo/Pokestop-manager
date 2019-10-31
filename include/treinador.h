#ifndef __TREINADOR__
#define __TREINADOR__

#include "./problem_params.h"

void* treinador(void* _ficha);

typedef struct Treinador {
  int id;
  int monstros[NUM_MONSTROS];
  Creche creches[NUM_CRECHES];
} Treinador;

Treinador newTreinador(int id, Creche creches[]);

#endif /* __TREINADOR__ */