#ifndef __LISTA_H__
#define __LISTA_H__

/**
 *Toma un puntero a void que representara un dato y devuelve otro
 *que contenga la misma informacion
*/
typedef void *(*FuncionCopiadora)(void *dato);

/**
 *Toma un puntero a void que representara un dato y libera la memoria de este.
*/
typedef void (*FuncionDestructora)(void *dato);

/**
 *Una Lista sera una lista doblemente enlazada que constara de un puntero a
 *NodoLista que apuntara al primer elemento de la lista, un puntero a NodoLista
 *que apuntara al nodo actual de la lista, un natural que indicara la cantidad
 *de elementos insertados, un natural que indicara la capacidad maxima,
 *una funcion copiadora y una funcion destructora.
 *Un NodoLista constara de un puntero a void que contenga el dato del nodo,
 *un puntero a NodoLista que apunte al nodo siguiente y otro que apunte al
 *anterior.
*/
typedef struct _NodoLista {
  void *dato;
  struct _NodoLista *ant;
  struct _NodoLista *sig;
} NodoLista;

struct _Lista {
  NodoLista *prm;               //primer nodo de la lista
  NodoLista *act;               //nodo actual
  unsigned nNodos;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionDestructora destr;
};

typedef struct _Lista *Lista;

//Crea una lista con la capacidad deseada y la devuelve
Lista lista_crear(unsigned, FuncionCopiadora, FuncionDestructora);

//Libera la memoria de una lista
void lista_destruir(Lista);

//Devuelve el largo total de una lista
int lista_largo(Lista);

//Inserta un elemento luego del puntero actual de la lista.
//Si tiene elementos siguientes a ese nodo los libera.
//Si la cantidad de elementos excede la capacidad libera el primero.
void lista_insertar(Lista, void *);

//Devuelve el dato de nodo al que apunta el puntero actual de la lista.
//Luego mueve el puntero actual al nodo anterior a este.
void *lista_anterior(Lista);

//Devuelve el dato del nodo siguiente al puntero actual y mueve el mismo a este.
//Si el puntero actual es NULL, devolvera el dato del primer puntero y actual
//Pasara a apuntar a este nodo.
void *lista_siguiente(Lista);

#endif
