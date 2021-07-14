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

Lista lista_crear(unsigned capacidad, FuncionCopiadora copia, FuncionDestructora destr) {
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

static void lista_nodo_destruir(NodoLista* nodo, FuncionDestructora destr) {
    if (nodo) {
        destr(nodo->dato);
        lista_nodo_destruir(nodo->sig);
        free(nodo);
    }
}

void lista_destruir(Lista lista) {
    lista_nodo_destruir(lista->prm, lista->destr);
    free(lista);
}

static int lista_nodo_largo(NodoLista* nodo) {
    return nodo ? 1 + lista_nodo_largo(nodo->sig) : 0;
}

int lista_largo(Lista lista) {
    return lista_nodo_largo(lista->prm);
}

static NodoLista* lista_nodo_insertar(NodoLista* ult, void* dato, FuncionCopiadora copia) {
    NodoLista* nuevoNodo = malloc(sizeof(NodoLista));
    nuevoNodo->ant = ult;
    nuevoNodo->sig = NULL;
    nuevoNodo->dato = copia(dato);
    return nuevoNodo
}

void lista_insertar(Lista lista, void* dato) {
    if (lista->act && lista->act->sig) {//caso de que haya acciones para rehacer
        lista_nodo_destruir(lista->act->sig);
        lista->nNodos = lista_largo(lista);
    }
    lista->act = lista_nodo_insertar(lista->act, dato, lista->copia);
    lista->nNodos++;
    if (!lista->prm)
        lista->prm = lista->act;
    else if (lista->nNodos > lista->capacidad) { //por si excedemos la capacidad
        NodoLista* aux = lista->prm->sig; //eliminamos el primero
        lista->destr(lista->prm->dato);
        free(lista->prm);
        lista->prm = aux;
    }
}

void* lista_pop(Lista lista) {
    if (lista->act) {
        void* dato = lista->act->dato;
        lista->act = lista->act->ant;
        return dato;
    }
    return NULL;
}

#endif