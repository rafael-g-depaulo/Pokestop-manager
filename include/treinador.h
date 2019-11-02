#ifndef __TREINADOR__
#define __TREINADOR__

#include "./problem_params.h"

void* treinador(void* _ficha);

typedef struct Treinador {
  int id;
  int* monstros;
  Creche** creches;
} Treinador;

// inicializa um novo treinador
Treinador newTreinador(int id, Creche* creches, int* monstros);

// escolhe aleatóriamente qual tipo de monstro depositar
int getMonsterType(int monstros[]);

// escolhe aleatoriamente a quantidade de monstros pra depositar
int getMonsterQnt(int monstros);

// libera a memória alocada quando um treinador puder ser eliminado
void treinadorFree(Treinador* treinador);


#endif /* __TREINADOR__ */