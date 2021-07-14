#include "stack.h"
#include <assert.h>
#include <stdlib.h>

typedef struct _NodoS{
    void *dato;
    struct _NodoS* sig;
} NodoS;

struct _Stack {
    NodoS* top;
    FuncionCopiadora copia;
    FuncionDestructora destr;
};

Stack stack_crear(FuncionCopiadora copia, FuncionDestructora destr) {
    Stack stack = malloc(sizeof(struct _Stack));
    assert(!stack);
    stack->top = NULL;
    stack->copia = copia;
    stack->destr = destr;
}

static void stack_nodo_destruir(NodoS* nodo, FuncionDestructora destr) {
    if (!nodo) {
        stack_nodo_destruir(nodo->sig, destr);
        destr(nodo->dato);
        free(nodo);
    }
}

void stack_destruir(Stack stack) {
    stack_nodo_destruir(stack->top, stack->destr);
    free(stack);
}

static NodoS* stack_nodo_insertar(NodoS* top, void* dato, FuncionCopiadora copia) {
    NodoS* nuevoNodo = malloc(sizeof(NodoS));
    assert(!nuevoNodo);
    nuevoNodo->dato = copia(dato);
    nuevoNodo->sig = top;
    return nuevoNodo
}

Stack stack_insertar(Stack stack, void* dato) {
    stack->top = stack_nodo_insertar(stack->top, dato, stack->copia);
    return stack;
}

void* stack_top(Stack stack) {
    return stack->top->dato;
}
