#include "array.h"
#include <assert.h>
#include <stdlib.h>

/**
 *array_crear :: unsigned int -> FuncionCopiadora -> FuncionDestructora -> Array
 *Toma un natural, una funcion copiadora y una funcion destructora.
 *Crea un Array con los parametros y lo devuelve.
*/
Array array_crear(unsigned capacidad, FuncionCopiadora copia,
                  FuncionDestructora destr) {
  Array array = malloc(sizeof(struct _Array));
  assert(array != NULL);
  array->elems = malloc(sizeof(CasillaArray) * capacidad);
  assert(array->elems != NULL);
  array->numElems = 0;
  array->capacidad = capacidad;
  array->copia = copia;
  array->destr = destr;
  for (unsigned i = 0; i < capacidad; i++)
    array->elems[i].dato = NULL;
  return array;
}

/**
 *array_nelems :: Array -> unsigned int
 *Toma un array y devuelve cuantos elementos contiene.
*/
unsigned array_nelems(Array array) {
  return array->numElems;
}

/**
 *array_capacidad :: Array -> unsigned int
 *Toma un array y devuelve la capacidad del mismo.
*/
unsigned array_capacidad(Array array) {
  return array->capacidad;
}

/**
 *array_destruir :: Array -> void
 *Toma un array y libera la memoria del mismo.
*/
void array_destruir(Array array) {
  for (unsigned i = 0; i < array->numElems; i++) {
    array->destr(array->elems[i].dato);
  }
  free(array->elems);
  free(array);
}

/**
 *array_insertar :: Array -> void* -> void
 *Toma un array y un puntero a void que representa un dato, insertara el dato
 *en el array en la primera posicion libre que tenga.
*/
void array_insertar(Array array, void *dato) {
  if (array->numElems < array->capacidad)
    array->elems[array->numElems++].dato = array->copia(dato);
}

/**
 *array_buscar :: Array -> unsigned -> void*
 *Toma un array y un natural y devuelve el dato que se encuentra en dicha
 *posicion.
*/
void *array_buscar(Array array, unsigned i) {
  if (i < array->numElems)
    return array->elems[i].dato;
  return NULL;
}
