#include "../src/avl.h"
#include "funciones_utiles_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
struct _Auxiliar {
    int* numeros;
    int indice;
};
typedef struct _Auxiliar *Auxiliar;

static void insertar_puntero_entero_en_Auxiliar(void* i, void* extra) {
  Auxiliar numeros = (Auxiliar) extra;
  numeros->numeros[numeros->indice++] = *(int*) i;
}

void test_avl() {
    {
    printf("TESTEANDO FUNCION avl_insertar\n");
    AVL arbol = avl_crear(copiar_puntero_entero, comparar_puntero_entero, destruir_puntero_entero);
  for (int i = 0; i < 500; ++i) {
    int i = rand() % 1000;
    avl_insertar(arbol, &i);
    assert(avl_validar(arbol) == 1);
  }
  avl_destruir(arbol);
    }
    {
        printf("TESTEANDO FUNCION avl_buscar\n");
        AVL arbol = avl_crear(copiar_puntero_entero, comparar_puntero_entero, destruir_puntero_entero);
        int numeros[] = {23, 56, 78, 21, 2, 35, 49, 64};
        for (int i = 0; i < 8; i++)
            avl_insertar(arbol, numeros + i);
        for (int i = 0; i < 8; i++) {
            assert((*(int*)avl_buscar(arbol, numeros + i)) == numeros[i]);
        }
        int aux = 58;
        assert(avl_buscar(arbol, &aux) == NULL);
        avl_destruir(arbol);
    }
    {
        printf("TESTEANDO FUNCION avl_recorrer\n");
        AVL arbol = avl_crear(copiar_puntero_entero, comparar_puntero_entero, destruir_puntero_entero);
        int numeros[] = {23, 56, 78, 21, 2, 35, 49, 64};
        for (int i = 0; i < 8; i++)
            avl_insertar(arbol, numeros + i);
        Auxiliar a = malloc(sizeof(struct _Auxiliar));
        a->numeros = malloc(sizeof(int*) * 8);
        a->indice = 0;
        avl_recorrer(arbol, 0, insertar_puntero_entero_en_Auxiliar, a);
        assert(a->indice = 8);
        assert(a->numeros[0] == 2);
        assert(a->numeros[1] == 21);
        assert(a->numeros[2] == 23);
        assert(a->numeros[3] == 35);
        assert(a->numeros[4] == 49);
        assert(a->numeros[5] == 56);
        assert(a->numeros[6] == 64);
        assert(a->numeros[7] == 78);
        free(a->numeros);
        free(a);
        avl_destruir(arbol);
    }
}
int main() {
    test_avl();
}
