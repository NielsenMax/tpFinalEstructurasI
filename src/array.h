#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef void *(*FuncionCopiadora)(void *);
/** Retorna una copia fisica del dato */

typedef void (*FuncionDestructora)(void *);
/** Libera la memoria alocada para el dato */

typedef struct _CasillaArray {
    void* dato;
} CasillaArray;

struct _Array {
    CasillaArray* elems;
    unsigned numElems;
    unsigned capacidad;
    FuncionCopiadora copia;
    //FuncionComparadora comp;
    FuncionDestructora destr;
};

typedef struct _Array *Array;

Array array_crear(unsigned, FuncionCopiadora copia, FuncionDestructora destr);

int array_nelems(Array);

int array_capacidad(Array);

void array_destruir(Array);

void array_insertar(Array, void*);

void* array_buscar(Array, unsigned);

#endif // __ARRAY_H__
