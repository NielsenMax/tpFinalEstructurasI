#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

/**
 *Un arbol AVL constara de una raiz y sus respectivas funciones:
 *- La funcion copia devolvera una copia de los datos de un nodo
 *- La funcion comp comparara dos elementos devolviendo 0 si son iguales,
 *  un numero negativo si el primero es menor y uno positivo si el segundo lo es
 *- La funcion destr libera la memoria de los datos de un nodo
 *Cada nodo cuenta con un puntero a un dato generico, dos punteros
 *a otros nodos AVL que seran sus hijos y un entero que representara la altura.
 *Un nodo hoja tendra altura 0, un nodo NULL tendra altura -1.
*/

typedef struct _AVL_Nodo {
  void *dato;
  struct _AVL_Nodo *izq, *der;
  int altura;
} AVL_Nodo;

struct _AVL {
  AVL_Nodo *raiz;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
};

/**
 *avl_crear :: FuncionCopiadora -> FuncionComparadora -> FuncionDestructora->AVL
 *Toma las funciones necesarias de un AVL y crea un nuevo AVL
 *con la raiz apuntando a NULL.
*/
AVL avl_crear(FuncionCopiadora copia, FuncionComparadora comp,
              FuncionDestructora destr) {
  AVL arbol = malloc(sizeof(struct _AVL));
  assert(arbol != NULL);        //Si no hay memoria en el sistema damos error
  arbol->raiz = NULL;
  arbol->copia = copia;
  arbol->comp = comp;
  arbol->destr = destr;
  return arbol;
}

/**
 *avl_nodo_destruir :: (AVL_Nodo*) -> FuncionDestructora -> void
 *Funcion estatica que toma un puntero a un nodo AVL y una funcion que libere
 *la memoria de su tipo de dato. Libera la memoria del dato del nodo con
 *la funcion, llama recursivamente a la liberacion de sus hijos y luego
 *libera al nodo en si.
*/
static void avl_nodo_destruir(AVL_Nodo * raiz, FuncionDestructora destr) {
  if (raiz) {
    avl_nodo_destruir(raiz->izq, destr);
    avl_nodo_destruir(raiz->der, destr);
    destr(raiz->dato);
    free(raiz);
  }
}

/**
 *avl_destruir :: AVL -> void
 *Toma un arbol AVL y libera toda su memoria
*/
void avl_destruir(AVL arbol) {
  avl_nodo_destruir(arbol->raiz, arbol->destr);
  free(arbol);
}

/**
 *avl_nodo_buscar :: (AVL_Nodo*) -> FuncionComparadora -> void* -> void*
 *Funcion estatica que toma un puntero a un nodo AVL, una funcion que compare
 *datos de los nodos entre si y un puntero void que contenga un dato. Devuelve
 *un puntero a void con el dato en el nodo para el cual la funcion devuelve 0
 *al comparardo con el dato pasado. Devuelve NULL si ninguno coincide.
*/
static void *avl_nodo_buscar(AVL_Nodo * raiz, FuncionComparadora comp,
                             void *dato) {
  if (!raiz)
    return NULL;
  else if (!comp(dato, raiz->dato))     //los datos son iguales
    return raiz->dato;
  else if (comp(dato, raiz->dato) < 0)  //el dato es menor que el de la raiz
    return avl_nodo_buscar(raiz->izq, comp, dato);
  else                          //el dato es mayor que el de la raiz
    return avl_nodo_buscar(raiz->der, comp, dato);
}

/**
 *avl_buscar :: AVL -> void* -> void*
 *Toma un arbol AVL y un puntero a void con el dato en el arbol que coincida
 *con el pasado. Devuelve NULL si ninguno conincide.
*/
void *avl_buscar(AVL arbol, void *dato) {
  return avl_nodo_buscar(arbol->raiz, arbol->comp, dato);
}

/**
 *avl_nodo_altura :: AVL_Nodo* -> int
 *Funcion estatica que toma un nodo AVL y devuelve a que altura se encuentra.
 *La altura es 0 si es una hoja, -1 si es NULL.
*/
static int avl_nodo_altura(AVL_Nodo * raiz) {
  return (raiz == NULL ? -1 : raiz->altura);
}

/**
 *avl_numero_nodos :: AVL -> int
 *Toma un arbol AVL y devuelve la cantidad maxima de nodos que puede contener
 *sin aumentar su altura. La formula para esto, con altura h, es:
 *2^(h+1) - 1.
*/
int avl_numero_nodos(AVL arbol) {
  int altura = avl_nodo_altura(arbol->raiz);
  return altura == -1 ? -1 : pow(2, altura + 1) - 1;
}

/**
 *avl_nodo_max_altura_hijos :: AVL_Nodo* -> unsigned int
 *Funcion estatica que toma un puntero a un nodo AVL y devuelve
 *la altura maxima entre sus hijos. Si el puntero es NULL da error.
*/
static unsigned int avl_nodo_max_altura_hijos(AVL_Nodo * raiz) {
  assert(raiz);
  int alturaIzq = avl_nodo_altura(raiz->izq);
  int alturaDer = avl_nodo_altura(raiz->der);
  return (alturaIzq < alturaDer ? alturaDer : alturaIzq);
}

/**
 *avl_nodo_factor_balance :: AVL_Nodo* -> int
 *Funcion estatica que toma un puntero a un nodo AVL y calcula su factor
 *de balance, es decir cual es la diferencia entre la altura de su hijo derecho
 *y su hijo izquierdo. Este numero deberia ser -1, 0 o 1. Para cualquier otro
 *valor el arbol no sera un AVL y dara error la funcion. Tambien dara error
 *si el puntero es NULL.
*/
static int avl_nodo_factor_balance(AVL_Nodo * raiz) {
  assert(raiz);
  int factor = avl_nodo_altura(raiz->der) - avl_nodo_altura(raiz->izq);
  assert(-2 <= factor || factor <= 2);
  return factor;
}

/**
 *avl_nodo_rotacion_simple_izq :: AVL_Nodo* -> AVL_Nodo*
 *Funcion estatica que toma un puntero a un nodo avl y devuelve el mismo
 *luego de realizarle una rotacion simple a izquierda, es decir
 *la raiz sera el hijo derecho, el nuevo hijo izquierdo sera la antigua raiz y
 *el nuevo hijo derecho sera el antiguo hijo izquierdo.
*/
static AVL_Nodo *avl_nodo_rotacion_simple_izq(AVL_Nodo * raiz) {
  AVL_Nodo *hijoDer = raiz->der;
  assert(hijoDer);
  raiz->der = hijoDer->izq;
  hijoDer->izq = raiz;
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoDer->altura = 1 + avl_nodo_max_altura_hijos(hijoDer);
  return hijoDer;
}

/**
 *avl_nodo_rotacion_simple_der :: AVL_Nodo* -> AVL_Nodo*
 *Funcion estatica que toma un puntero a un nodo avl y devuelve el mismo
 *luego de realizarle una rotacion simple a derecha, es decir
 *la raiz sera el hijo izquierdo, el nuevo hijo izquierdo sera el antiguo hijo
 *derecho y el nuevo hijo derecho sera la antigua raiz.
*/
static AVL_Nodo *avl_nodo_rotacion_simple_der(AVL_Nodo * raiz) {
  AVL_Nodo *hijoIzq = raiz->izq;
  assert(hijoIzq);
  raiz->izq = hijoIzq->der;
  hijoIzq->der = raiz;
  raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
  hijoIzq->altura = 1 + avl_nodo_max_altura_hijos(hijoIzq);
  return hijoIzq;
}

/**
 *avl_nodo_crear :: void* -> FuncionCopiadora -> AVL_Nodo*
 *Funcion estatica que toma un puntero a void que contiene un dato, una funcion
 *copiadora y devuelve un nodo AVL con el dato pasado como dato del nodo.
*/
static AVL_Nodo *avl_nodo_crear(void *dato, FuncionCopiadora copia) {
  AVL_Nodo *nuevoNodo = malloc(sizeof(AVL_Nodo));
  assert(nuevoNodo);
  nuevoNodo->dato = copia(dato);
  nuevoNodo->izq = nuevoNodo->der = NULL;
  nuevoNodo->altura = 0;
  return nuevoNodo;
}

/**
 *avl_nodo_insertar :: AVL_Nodo* -> void* ->FuncionCopiadora->FuncionComparadora
 *Funcion estatica que toma un puntero a un nodo AVL, un puntero a void que sera
 *el dato a insertar, una funcion copiadora y una funcion comparadora.
 *La funcion devolvera un nodo AVL que contenga al dato pasado como una
 *como hoja. La insercion se realiza mantiendo el orden entre los nodos, es
 *decir hijos izquierdos < raiz < hijos derechos, y tambien mantiene la
 *propiedad de que el arbol sea completo es decir que para todo nodo interno
 *la diferencia de altura de sus dos arboles hijos esta entre -1 y 1.
*/
static AVL_Nodo *avl_nodo_insertar(AVL_Nodo * raiz, void *dato,
                                   FuncionCopiadora copia,
                                   FuncionComparadora comp) {
  if (!raiz)                    //Si es NULL es que se puede insertar
    return avl_nodo_crear(dato, copia);
  else if (comp(dato, raiz->dato) < 0) {        //el dato es menor que la raiz
    raiz->izq = avl_nodo_insertar(raiz->izq, dato, copia, comp);
    //El arbol izquierdo tiene mayor altura que el derecho
    if (avl_nodo_factor_balance(raiz) == -2) {
      //el subarbol derecho tiene mayor altura que el subarbol izquierdo
      if (avl_nodo_factor_balance(raiz->izq) == 1)
        raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
      raiz = avl_nodo_rotacion_simple_der(raiz);
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz); //nueva altura
    return raiz;
  } else if (comp(raiz->dato, dato) < 0) {      //el dato es mayor
    raiz->der = avl_nodo_insertar(raiz->der, dato, copia, comp);
    //el arbol derecho tiene mayor altura que el izquierdo
    if (avl_nodo_factor_balance(raiz) == 2) {
      //el subarbol izquierdo tiene mayor altura que el derecho
      if (avl_nodo_factor_balance(raiz->der) == -1)
        raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
      raiz = avl_nodo_rotacion_simple_izq(raiz);
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  } else {
    return raiz;                // si el dato es igual no realizamos nada
  }
}

/**
 *avl_insertar :: AVL -> void* -> void
 *Toma un arbol AVL y un puntero a void que contiene un dato.
 *La funcion inserta el nuevo dato, segun corresponda, en el AVL.
*/
void avl_insertar(AVL arbol, void *dato) {
  arbol->raiz =
      avl_nodo_insertar(arbol->raiz, dato, arbol->copia, arbol->comp);
}

/**
 *avl_nodo_validar_abb :: AVL_Nodo* -> void* -> void* -> FuncionComparadora->int
 *Funcion estatica que toma un puntero a un nodo AVL, dos punteros a void
 *que contienen el dato que deberian ser minimo y maximo del arbol y una
 *funcion comparadora. La funcion controla que toda raiz sea mayor que el
 *minimo pasado y menor que el maximo. Si esto no se cumple devuelve 0,
 *si se cumple 1.
*/
static int avl_nodo_validar_abb(AVL_Nodo * raiz, void *min, void *max,
                                FuncionComparadora comp) {
  if (!raiz)
    return 1;
  else {
    if (min && comp(raiz->dato, min) <= 0)
      return 0;
    if (max && comp(max, raiz->dato) <= 0)
      return 0;
    return (avl_nodo_validar_abb(raiz->izq, min, raiz->dato, comp))
        && (avl_nodo_validar_abb(raiz->der, raiz->dato, max, comp));
  }
}

/**
 *avl_nodo_hijo_mas_izquierda :: AVL_Nodo* -> AVL_Nodo*
 *Funcion estatica que toma un puntero a un nodo AVL y devuelve
 *otro puntero a un nodo AVL que sera el hijo mas a la izquerda del nodo pasado.
*/
static AVL_Nodo *avl_nodo_hijo_mas_izquierda(AVL_Nodo * raiz) {
  if (!raiz)
    return NULL;
  if (!raiz->izq)               //Si su hijo izquierdo es NULL no se puede ir mas a la izq
    return raiz;
  else
    return avl_nodo_hijo_mas_izquierda(raiz->izq);
}

/**
 *avl_nodo_eliminar :: AVL_Nodo* -> void* -> FuncionComparadora ->
 *                     FucionCopiadora -> FuncionDestructora ->AVL_Nodo*
 *Funcion estatica que tomara un puntero a un nodo AVL, un puntero a void
 *que sera el dato a eliminar y las tres funciones de un AVL.
 *La funcion devolvera un puntero a nodo AVL que no
 *contenga al dato pasado como nodo interno ni como hoja.
*/
static AVL_Nodo *avl_nodo_eliminar(AVL_Nodo * raiz, void *dato,
                                   FuncionComparadora comp,
                                   FuncionCopiadora copia,
                                   FuncionDestructora destr) {
  if (raiz == NULL)             //el dato no estaba en el arbol
    return NULL;
  else if (!comp(dato, raiz->dato)) {   //dato esta en raiz
    //si el hijo izq es NULL tomo el der, si der es NULL devolvera NULL
    if (!raiz->izq) {
      AVL_Nodo *hijoDer = raiz->der;
      destr(raiz->dato);        //destruimos el dato de la raiz
      free(raiz);
      return hijoDer;           //devolvemos el hijo derecho
    } else if (!raiz->der) {    //si el hijo derecho es NULL
      AVL_Nodo *hijoIzq = raiz->izq;
      destr(raiz->dato);
      free(raiz);
      return hijoIzq;           //devolvemos el hijo izquierdo
    } else {                    //la raiz tiene ambos hijos
      //elegimos el hijo mas a la izquierda del subarbol derecho como raiz
      AVL_Nodo *nuevaRaiz = avl_nodo_hijo_mas_izquierda(raiz->der);
      destr(raiz->dato);
      //ponemos al nuevo dato en la raiz
      raiz->dato = copia(nuevaRaiz->dato);
      //eliminamos al anterior nodo que contenia a la nueva raiz
      raiz->der =
          avl_nodo_eliminar(raiz->der, nuevaRaiz->dato, comp, copia,
                            destr);
      //balanceamos el arbol
      if (avl_nodo_factor_balance(raiz) == -2) {
        if (avl_nodo_factor_balance(raiz->izq) == 1)
          raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
        raiz = avl_nodo_rotacion_simple_der(raiz);
      } else if (avl_nodo_factor_balance(raiz) == 2) {
        if (avl_nodo_factor_balance(raiz->der) == -1)
          raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
        raiz = avl_nodo_rotacion_simple_izq(raiz);
      }
      raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);       //nueva altura
      return raiz;
    }
  } else if (comp(dato, raiz->dato) < 0) {      //el dato es menor a la raiz
    raiz->izq = avl_nodo_eliminar(raiz->izq, dato, comp, copia, destr);
    if (avl_nodo_factor_balance(raiz) == 2) {
      if (avl_nodo_factor_balance(raiz->der) == -1)
        raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
      raiz = avl_nodo_rotacion_simple_izq(raiz);
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  } else {                      //el dato es mayor que la raiz
    raiz->der = avl_nodo_eliminar(raiz->der, dato, comp, copia, destr);
    if (avl_nodo_factor_balance(raiz) == -2) {
      if (avl_nodo_factor_balance(raiz->izq) == 1)
        raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
      raiz = avl_nodo_rotacion_simple_der(raiz);
    }
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    return raiz;
  }
}

/**
 *avl_eliminar :: AVL -> void* -> void
 *Toma un arbol AVL y un dato que eliminara del arbol
 *si este pertenece al arbol.
*/
void avl_eliminar(AVL arbol, void *dato) {
  arbol->raiz =
      avl_nodo_eliminar(arbol->raiz, dato, arbol->comp, arbol->copia,
                        arbol->destr);
}

/**
 *avl_nodo_validar_altura_y_balance :: AVL_Nodo*
 *Funcion estatica que toma un puntero a un nodo AVL y comprueba que su altura
 *este bien calculada y el factor de balance del nodo este en {-1, 0, 1}.
 *Devuelve un 1 si cumple las condiciones, 0 sino.
*/
static int avl_nodo_validar_altura_y_balance(AVL_Nodo * raiz) {
  if (!raiz)
    return 1;
  int ret1 = avl_nodo_validar_altura_y_balance(raiz->izq);
  int ret2 = avl_nodo_validar_altura_y_balance(raiz->der);
  if (ret1 && ret2) {
    int altura = 1 + avl_nodo_max_altura_hijos(raiz);
    int balance = avl_nodo_factor_balance(raiz);
    if ((raiz->altura == altura) && (balance >= -1) && (balance <= 1))
      return 1;
  }
  return 0;
}

/**
 *avl_validar :: AVL -> int
 *Toma un arbol AVL y comprueba que este bien balanceado y sus nodos esten bien
 *insertados. Devuelve 1 si cumple ambas condiciones, 0 sino.
*/
int avl_validar(AVL arbol) {
  return (avl_nodo_validar_altura_y_balance(arbol->raiz)
          && avl_nodo_validar_abb(arbol->raiz, NULL, NULL, arbol->comp));
}

/**
 *avl_nodo_recorrer::AVL_Nodo*->AVLRecorrido->Funcionvisitanteextra->void*->void
 *Funcion estatica que toma un puntero a nodo AVL, un natural perteneciente
 *a AVLRecorrido (entre 0 y 2), una funcion visitante y un puntero a void que
 *contega datos extras para la funcion visitante. La funcion recorrera el arbol
 *en inorden si el natural es 0, en preorden si es 1 y en postorden si es 2,
 *aplicando la funcion visitante a cada dato de cada nodo.
*/
static void avl_nodo_recorrer(AVL_Nodo * raiz, AVLRecorrido orden,
                              FuncionVisitanteExtra visita, void *extra) {
  if (raiz != NULL) {
    if (orden == AVL_RECORRIDO_PRE)
      visita(raiz->dato, extra);

    avl_nodo_recorrer(raiz->izq, orden, visita, extra);

    if (orden == AVL_RECORRIDO_IN)
      visita(raiz->dato, extra);

    avl_nodo_recorrer(raiz->der, orden, visita, extra);
    if (orden == AVL_RECORRIDO_POST)
      visita(raiz->dato, extra);
  }
}

/**
 *avl_recorrer :: AVL -> AVLRecorrido -> FuncionVisitanteExtra -> void* -> void
 *La funcion tomara un arbol AVL, un natural AVLRecorrido, una funcion visitante
 *y un puntero a void con datos extras para la funcion visitante.
 *Recorrera el arbol aplicando la funcion visitante a los datos segun el valor
 *del natural pasado.
*/
void avl_recorrer(AVL arbol, AVLRecorrido orden,
                  FuncionVisitanteExtra visita, void *extra) {
  avl_nodo_recorrer(arbol->raiz, orden, visita, extra);
}
