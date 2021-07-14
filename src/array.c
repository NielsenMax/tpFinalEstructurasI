#include "array.h"
#include <assert.h>
#include <stdlib.h>

Array array_crear(unsigned capacidad, FuncionCopiadora copia,
                  FuncionDestructora destr) {
    Array array = malloc(sizeof(struct _Array));
    assert(array != NULL);
    array->elems = malloc(sizeof(CasillaArray) * capacidad);
    assert(array->elems != NULL);
    array->numElems = 0;
    array->capacidad = capacidad;
    array->copia = copia;
    //array->comp = comp;
    array->destr = destr;
    for (unsigned i = 0; i < capacidad; i++)
        array->elems[i].dato = NULL;
    return array;
}

int array_nelems(Array array) {return array->numElems;}

int array_capacidad(Array array) {return array->capacidad;}

void array_destruir(Array array) {
    for (unsigned i = 0; i < array->numElems; i++) {
        array->destr(array->elems[i].dato);
    }
    free(array->elems);
    free(array);
}

void array_insertar(Array array, void *dato) {
    if (array->numElems < array->capacidad)
        array->elems[array->numElems++].dato = array->copia(dato);
}

void* array_buscar(Array array, unsigned i) {
    if (i < array->numElems)
        return array->elems[i].dato;
    return NULL;
}
