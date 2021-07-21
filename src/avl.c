#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct _AVL_Nodo {
    void* dato;
    struct _AVL_Nodo* izq, * der;
    int altura;
} AVL_Nodo;

struct _AVL {
    AVL_Nodo* raiz;
    FuncionCopiadora copia;
    FuncionComparadora comp;
    FuncionDestructora destr;
};

AVL avl_crear(FuncionCopiadora copia, FuncionComparadora comp, FuncionDestructora destr) {
    AVL arbol = malloc(sizeof(struct _AVL));
    assert(arbol != NULL);
        arbol->raiz = NULL;
        arbol->copia = copia;
        arbol->comp = comp;
        arbol->destr = destr;
    return arbol;
}

static void avl_nodo_destruir(AVL_Nodo* raiz, FuncionDestructora destr) {
    if (raiz) {
        avl_nodo_destruir(raiz->izq, destr);
        avl_nodo_destruir(raiz->der,destr);
        destr(raiz->dato);
        free(raiz);
    }
}
void avl_destruir(AVL arbol) {
    avl_nodo_destruir(arbol->raiz, arbol->destr);
    free(arbol);
}

static void* avl_nodo_buscar(AVL_Nodo* raiz, FuncionComparadora comp, void* dato) {
    if (!raiz)
        return NULL;
    else if (!comp(dato, raiz->dato))
        return raiz->dato;
    else if (comp(dato, raiz->dato) < 0)
        return avl_nodo_buscar(raiz->izq, comp, dato);
    else
        return avl_nodo_buscar(raiz->der, comp, dato);
}

void* avl_buscar(AVL arbol, void* dato) {
    return avl_nodo_buscar(arbol->raiz, arbol->comp, dato);
}

static int avl_nodo_altura(AVL_Nodo* raiz) {
    return (raiz == NULL ? -1 : raiz->altura);
}

int avl_numero_nodos(AVL arbol) {
    int altura = avl_nodo_altura(arbol->raiz);
    return  altura == -1? -1 : pow(2, altura + 1);
}

static unsigned int avl_nodo_max_altura_hijos(AVL_Nodo* raiz) {
    assert(raiz);
    int alturaIzq = avl_nodo_altura(raiz->izq);
    int alturaDer = avl_nodo_altura(raiz->der);
    return (alturaIzq < alturaDer ? alturaDer : alturaIzq);
}

static int avl_nodo_factor_balance(AVL_Nodo* raiz) {
    assert(raiz);
    int factor = avl_nodo_altura(raiz->der) - avl_nodo_altura(raiz->izq);
    assert(-2 <= factor || factor <= 2);
    return factor;
}

static AVL_Nodo* avl_nodo_rotacion_simple_izq(AVL_Nodo* raiz) {
    AVL_Nodo* hijoDer = raiz->der;
    assert(hijoDer);
    raiz->der = hijoDer->izq;
    hijoDer->izq = raiz;
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    hijoDer->altura = 1 + avl_nodo_max_altura_hijos(hijoDer);
    return hijoDer;
}

static AVL_Nodo* avl_nodo_rotacion_simple_der(AVL_Nodo* raiz) {
    AVL_Nodo* hijoIzq = raiz->izq;
    assert(hijoIzq);
    raiz->izq = hijoIzq->der;
    hijoIzq->der = raiz;
    raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
    hijoIzq->altura = 1 + avl_nodo_max_altura_hijos(hijoIzq);
    return hijoIzq;
}

static AVL_Nodo* avl_nodo_crear(void* dato, FuncionCopiadora copia) {
    AVL_Nodo* nuevoNodo = malloc(sizeof(AVL_Nodo));
    assert(nuevoNodo);
    nuevoNodo->dato = copia(dato);
    nuevoNodo->izq = nuevoNodo->der = NULL;
    nuevoNodo->altura = 0;
    return nuevoNodo;
}

static AVL_Nodo* avl_nodo_insertar(AVL_Nodo* raiz, void* dato, FuncionCopiadora copia, FuncionComparadora comp) {
    if (!raiz)
        return avl_nodo_crear(dato, copia);
    else if (comp(dato, raiz->dato) < 0) {
        raiz->izq = avl_nodo_insertar(raiz->izq, dato, copia, comp);
        if (avl_nodo_factor_balance(raiz) == -2) {
            if (avl_nodo_factor_balance(raiz->izq) == 1)
                raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
            raiz = avl_nodo_rotacion_simple_der(raiz);
        }
        raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
        return raiz;
    } else if (comp(raiz->dato, dato) < 0) {
        raiz->der = avl_nodo_insertar(raiz->der, dato, copia, comp);
        if (avl_nodo_factor_balance(raiz) == 2) {
            if (avl_nodo_factor_balance(raiz->der) == -1)
                raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
            raiz = avl_nodo_rotacion_simple_izq(raiz);
        }
        raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
        return raiz;
    }
    else {
        //destr(raiz->dato);
        //raiz->dato = copia(dato);
        return raiz;
    }
}

void avl_insertar(AVL arbol, void* dato) {
    arbol->raiz = avl_nodo_insertar(arbol->raiz, dato, arbol->copia, arbol->comp);
}

static int avl_nodo_validar_abb(AVL_Nodo* raiz, void* min, void* max, FuncionComparadora comp) {
    if (!raiz)
        return 1;
    else {
        if (min && comp(raiz->dato, min) <= 0)
            return 0;
        if (max && comp(max, raiz->dato) <= 0)
            return 0;
        return (avl_nodo_validar_abb(raiz->izq, min, raiz->dato, comp)) && (avl_nodo_validar_abb(raiz->der, raiz->dato, max, comp));
    }
}

static AVL_Nodo* avl_nodo_hijo_mas_izquierda(AVL_Nodo* raiz) {
    if (!raiz)
        return NULL;
    if (!raiz->izq)
        return raiz;
    else
        return avl_nodo_hijo_mas_izquierda(raiz->izq);
}

static AVL_Nodo* avl_nodo_eliminar(AVL_Nodo* raiz, void* dato, FuncionComparadora comp, FuncionCopiadora copia,FuncionDestructora destr) {
    if (raiz == NULL) //raiz es null
        return NULL;
    else if (!comp(dato, raiz->dato)) { //dato esta en raiz
        if (!raiz->izq) { //si el hijo izq es NULL tomo el der, si der es null devolvera NULL
            AVL_Nodo* hijoDer = raiz->der;
            //avl_nodo_destruir(raiz,destr);
            destr(raiz->dato);
            free(raiz);
            return hijoDer;
        } else if (!raiz->der) {
            AVL_Nodo* hijoIzq = raiz->izq;
            //avl_nodo_destruir(raiz,destr);
            destr(raiz->dato);
            free(raiz);
            return hijoIzq;
        } else {
            AVL_Nodo* nuevaRaiz = avl_nodo_hijo_mas_izquierda(raiz->der);
            destr(raiz->dato);
            raiz->dato = copia(nuevaRaiz->dato);
            raiz->der = avl_nodo_eliminar(raiz->der, nuevaRaiz->dato, comp, copia, destr);
            if (avl_nodo_factor_balance(raiz) == -2) {
                if (avl_nodo_factor_balance(raiz->izq) == 1)
                    raiz->izq = avl_nodo_rotacion_simple_izq(raiz->izq);
                raiz = avl_nodo_rotacion_simple_der(raiz);
            } else if (avl_nodo_factor_balance(raiz) == 2) {
                if (avl_nodo_factor_balance(raiz->der) == -1)
                    raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
                raiz = avl_nodo_rotacion_simple_izq(raiz);
            }
            raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
            return raiz;
        }
    } else if (comp(dato, raiz->dato) < 0) {
        raiz->izq = avl_nodo_eliminar(raiz->izq, dato, comp, copia, destr);
         if (avl_nodo_factor_balance(raiz) == 2) {
            if (avl_nodo_factor_balance(raiz->der) == -1)
                raiz->der = avl_nodo_rotacion_simple_der(raiz->der);
            raiz = avl_nodo_rotacion_simple_izq(raiz);
        }
        raiz->altura = 1 + avl_nodo_max_altura_hijos(raiz);
        return raiz;
    } else {
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

void avl_eliminar(AVL arbol, void* dato) {
    arbol->raiz = avl_nodo_eliminar(arbol->raiz, dato, arbol->comp, arbol->copia, arbol->destr);
}

static int avl_nodo_validar_altura_y_balance(AVL_Nodo* raiz) {
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

int avl_validar(AVL arbol) {
    return (avl_nodo_validar_altura_y_balance(arbol->raiz) && avl_nodo_validar_abb(arbol->raiz, NULL, NULL, arbol->comp));
}

static void avl_nodo_recorrer(AVL_Nodo* raiz, AVLRecorrido orden, FuncionVisitanteExtra visita, void* extra) {
    if (raiz != NULL) {
        if (orden == AVL_RECORRIDO_PRE)
            visita(raiz->dato, extra);
        // printf("(");
        avl_nodo_recorrer(raiz->izq, orden, visita, extra);

        if (orden == AVL_RECORRIDO_IN)
            visita(raiz->dato, extra);

        avl_nodo_recorrer(raiz->der, orden, visita, extra);
        //printf(")");
        if (orden == AVL_RECORRIDO_POST)
            visita(raiz->dato, extra);
    }
}

void avl_recorrer(AVL arbol, AVLRecorrido orden, FuncionVisitanteExtra visita, void* extra) {
     // printf("Raiz: ");
     // if (arbol->raiz)
     //    visita(arbol->raiz->dato, NULL);
    avl_nodo_recorrer(arbol->raiz, orden, visita, extra);
}
