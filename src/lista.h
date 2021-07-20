#ifndef __LISTA_H__
#define __LISTA_H__

typedef void *(*FuncionCopiadora)(void *dato);
/** Retorna una copia fisica del dato */
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef void (*FuncionDestructora)(void *dato);

typedef struct _NodoLista{
    void* dato;
    struct _NodoLista* ant;
    struct _NodoLista* sig;
} NodoLista;

struct _Lista {
    NodoLista* prm; //primer nodo de la lista
    NodoLista* act; //nodo actual
    unsigned nNodos;
    unsigned capacidad;
    FuncionCopiadora copia;
    FuncionDestructora destr;
};

typedef struct _Lista *Lista;

Lista lista_crear(unsigned, FuncionCopiadora, FuncionDestructora);

void lista_destruir(Lista);

int lista_largo(Lista);

void lista_insertar(Lista, void*);

void* lista_anterior(Lista);

void* lista_siguiente(Lista);

#endif
