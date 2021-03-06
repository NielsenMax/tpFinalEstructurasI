#include "lista.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 *lista_crear :: unsigned int -> FuncionCopiadora -> FuncionDestructora -> Lista
 *Toma un natural, una funcion copiadora y una funcion destructura y crea
 *una lista doblemente enlazada que sera retornada.
*/
Lista lista_crear(unsigned capacidad, FuncionCopiadora copia,
                  FuncionDestructora destr) {
  Lista lista = malloc(sizeof(struct _Lista));
  assert(lista);
  lista->nNodos = 0;
  lista->capacidad = capacidad;
  lista->prm = NULL;
  lista->act = NULL;
  lista->copia = copia;
  lista->destr = destr;
  return lista;
}

/**
 *lista_nodo_destruir :: NodoLista* -> FuncionDestructora -> void
 *Funcion estatica que toma un puntero de nodo de lista y una funcion
 *destructora y libera la memoria del mismo, llama recursivamente a
 *los nodos siguientes.
*/
static void lista_nodo_destruir(NodoLista * nodo, FuncionDestructora destr) {
  if (nodo) {
    destr(nodo->dato);
    lista_nodo_destruir(nodo->sig, destr);
    free(nodo);
  }
}

/**
 *lista_destruir :: Lista -> void
 *Toma una lista y libera la memoria de la misma.
*/
void lista_destruir(Lista lista) {
  lista_nodo_destruir(lista->prm, lista->destr);
  free(lista);
}

/**
 *lista_nodo_largo :: NodoLista* -> int
 *Funcion estatica que toma un puntero a un nodo lista y devuelve la cantidad
 *de nodos siguientes que le siguen al pasado. Si el nodo es NULL devuelve 0.
*/
static int lista_nodo_largo(NodoLista * nodo) {
  return nodo ? 1 + lista_nodo_largo(nodo->sig) : 0;
}

/**
 *lista_largo :: Lista -> int
 *Toma una lista y devuelve la cantida de elementos que contiene.
*/
int lista_largo(Lista lista) {
  return lista_nodo_largo(lista->prm);
}

/**
 *lista_nodo_insertar :: NodoLista* -> void* -> FuncionCopiadora ->NodoLista*
 *Funcion estatica que toma un puntero a un nodo lista, un puntero a void
 *que representa un dato y una funcion copiadora.
 *La funcion inserta el dato pasado como el nodo siguiente al pasado.
*/
static NodoLista *lista_nodo_insertar(NodoLista * ult, void *dato,
                                      FuncionCopiadora copia) {
  NodoLista *nuevoNodo = malloc(sizeof(NodoLista));
  nuevoNodo->ant = ult;
  nuevoNodo->sig = NULL;
  nuevoNodo->dato = copia(dato);
  if (ult)
    ult->sig = nuevoNodo;
  return nuevoNodo;
}

/**
 *lista_insertar :: Lista -> void* -> void
 *Toma una lista y un puntero a void que representa un dato y lo inserta
 *luego del nodo actual. Si hay nodos posteriores al actual los libera y
 *luego inserta el dato. Luego de insertarlo mueve el puntero al nodo actual
 *para que apunte al nuevo elemento. Si la lista estaba vacia el puntero al
 *primer nodo se actualiza.
 *Si al insertar el nodo la cantidad de elementos es mayor a la capacidad
 *se libera el primer elemento de la lista.
*/
void lista_insertar(Lista lista, void *dato) {
  if (lista->capacidad) {       //por si la capacidad es 0
    if (lista->act == NULL) {   //Podria haber acciones a rehacer
      if (lista->prm) {         //Si las hay las eliminamos
        lista_nodo_destruir(lista->prm, lista->destr);
        lista->prm = NULL;
        lista->nNodos = 0;
      }
    } else {
      if (lista->act->sig) {    //vemos si hay acciones a rehacer
        lista_nodo_destruir(lista->act->sig, lista->destr);
        lista->act->sig = NULL;
        lista->nNodos = lista_largo(lista);
      }
    }
    lista->act = lista_nodo_insertar(lista->act, dato, lista->copia);
    lista->nNodos++;
    if (!lista->prm)            //si prm es null lo ponemos en el elemento actual
      lista->prm = lista->act;
    if (lista->nNodos > lista->capacidad) {     //Si nos pasamos de capacidad
      NodoLista *aux = lista->prm->sig; //eliminamos el primero
      lista->destr(lista->prm->dato);
      free(lista->prm);
      lista->prm = aux;
      lista->nNodos--;
    }
  }
}

/**
 *lista_anterior :: Lista -> void*
 *Toma una lista y devuelve el dato al cual apunta el puntero actual
 *de la lista.
 *Luego mueve el puntero actual al nodo anterior, si este no existe
 *el puntero actual pasara a ser NULL.
*/
void *lista_anterior(Lista lista) {
  if (lista->act) {
    void *dato = lista->act->dato;
    lista->act = lista->act->ant;
    return dato;
  }
  return NULL;
}

/**
 *lista_siguiente :: Lista -> void*
 *Toma una lista y devuelve el dato del nodo siguiente al cual apunta el
 *el puntero actual de la lista. Si el puntero actual es NULL usa el puntero
 *al primer elemento.
 *Luego mueve el puntero actual al nodo siguiente.
 *Si la lista esta vacia o no tiene un nodo siguiente devuelve NULL.
*/
void *lista_siguiente(Lista lista) {
  if (lista->act) {
    lista->act = lista->act->sig;
    return lista->act ? lista->act->dato : NULL;
  } else {
    lista->act = lista->prm;
    return lista->act ? lista->act->dato : NULL;
  }
}
