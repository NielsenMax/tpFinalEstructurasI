#include <stdio.h>
#include <stdlib.h>

void* copiar_puntero_entero(void* i) {
  int* p = malloc(sizeof(int));
  *p = *(int*)i;
  return p;
}
int comparar_puntero_entero(void* i1, void* i2) {
  return (*(int*)i1 - *(int*)i2);
}

void destruir_puntero_entero(void* i) { free(i); }
