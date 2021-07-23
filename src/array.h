#ifndef __ARRAY_H__
#define __ARRAY_H__

/**
 *Toma un puntero a void que representara un dato y devuelve otro
 *que contenga la misma informacion
*/
typedef void *(*FuncionCopiadora)(void *);

/**
 *Toma un puntero a void que representara un dato y libera la memoria de este.
*/
typedef void (*FuncionDestructora)(void *);

/**
 *Un array constara de un natural que contenga la cantidad de elementos
 *insertados, otro que indicara la capacidad maxima, una funcion
 *copiadora, una funcion destructora y un puntero a CasillaArray
 *que contendra los datos del mismo.
*/
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

//Puntero a struct _Array para facilitar el uso
typedef struct _Array *Array;

//Crea un array apartir de los parametros necesarios y lo devuelve
Array array_crear(unsigned, FuncionCopiadora copia, FuncionDestructora destr);

//Devuelve la cantidad de elementos de un array
unsigned array_nelems(Array);

//Devuelve la capacidad de un array
unsigned array_capacidad(Array);

//Libera la memoria de un array
void array_destruir(Array);

//Inserta un elemento al final de un array
void array_insertar(Array, void*);

//Devuelve el elemento de la posicion pasada de un array
void* array_buscar(Array, unsigned);

#endif // __ARRAY_H__
