#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "treinador.h"

int main () {

  pthread_t threads[5];
  
  for (long int i = 0; i < 5; i++)
    pthread_create(&threads[i], NULL, thread, (void*) i);

  for (int i = 0; i < 5; i++)
    pthread_join(threads[i], NULL);

  return 0;
}