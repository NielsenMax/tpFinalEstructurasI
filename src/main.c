#include "acciones.h"
#include "contacto.h"
#include "avl.h"
#include "archivo.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_ACCIONES 20

void imprimir_lista_nodo(NodoLista* nodo) {
    if (nodo) {
        if (nodo->dato)
            accion_imprimir(nodo->dato);
        imprimir_lista_nodo(nodo->sig);
    } else
        printf("\nFINAL\n");
}

void menu() {
    printf("Menu de acciones:\n");
    printf("1. Buscar\n");
    printf("2. Agregar\n");
    printf("3. Eliminar\n");
    printf("4. Editar\n");
    printf("5. Cargar\n");
    printf("6. Guardar\n");
    printf("7. Deshacer\n");
    printf("8. Rehacer\n");
    printf("9. AND\n");
    printf("10. OR\n");
    printf("11. Guardar ordenado\n");
    printf("12. Buscar por suma de edades\n");
    printf("13. Salir\n");
    printf("\n");
}

int main() {
    // Lista lista = lista_crear(MAX_ACCIONES, accion_copia, accion_destruir);
    // Contacto c = contacto_crear("maxi", "nielsen", "3413190720",20);
    // Accion a = accion_crear(c, "", 1);
    // lista_insertar(lista, a);
    // contacto_destruir(c);
    // accion_destruir(a);
    // // c = contacto_crear("maxi", "nielsen", "3413190720",20);
    // a = accion_crear(NULL, "hola.csv", 2);
    // lista_insertar(lista, a);
    // imprimir_lista_nodo(lista->prm);
    // accion_imprimir(lista_anterior(lista));
    // accion_imprimir(lista->act->sig->dato);
    // Accion b = lista_siguiente(lista);
    // if (b)
    //     accion_imprimir(b);
    // AVL arbol = avl_crear(contacto_copia, contacto_comparar_nombre, contacto_destruir);
    // Contacto c = contacto_crear("maxi", "nielsen", "3413190720",20);
    // avl_insertar(arbol, c);
    // contacto_destruir(c);
    //  c = contacto_crear("alejo", "gonzalez", "3413190720",25);
    // avl_insertar(arbol, c);
    // contacto_destruir(c);
    // c = contacto_crear("alejo", "a", "3413190720",21);
    // avl_insertar(arbol, c);
    // contacto_destruir(c);
    // c = contacto_crear("z", "a", "3413190720",19);
    // avl_insertar(arbol, c);
    // contacto_destruir(c);
    // c = contacto_crear("z", "b", "3413190720",30);
    // avl_insertar(arbol, c);
    // contacto_destruir(c);
    // c = contacto_crear("z","a", "", 0);
    // avl_eliminar(arbol,c);
    // contacto_destruir(c);
    // avl_recorrer(arbol, 0, contacto_imprimir, NULL);
    // printf("\n");

    // AVL arbol_edades = avl_crear(contacto_copia, contacto_comparar_edad, contacto_destruir);
    // avl_recorrer(arbol, 0, contacto_agregar_avl, arbol_edades);
    // avl_recorrer(arbol_edades, 0, contacto_imprimir, NULL);
    // printf("\n");
    // Array array = array_crear(50, contacto_copia, contacto_destruir);
    // c = contacto_crear("z", "b", "3413190720",30);
    //  array_insertar(array, c);
    //  array_insertar(array, c);
    //  contacto_imprimir(array_buscar(array, 1), NULL);
    // contacto_destruir(c);

    // avl_recorrer(arbol, 0, contacto_agregar_array, array);
    // for (unsigned i = 0; i < array->numElems; i++) {
    //     contacto_imprimir(array_buscar(array, i), NULL);
    //     printf("\n");
    // }
    // Array subConj = suma_edades(array, 0);
    // if (array_nelems(subConj)) {
    //     for (unsigned i = 0; i < subConj->numElems; i++) {
    //     contacto_imprimir(array_buscar(subConj, i), NULL);
    //     printf("\n");
    //     }
    // } else
    //       printf("No existe el subconjunto");

    // c = contacto_crear("z","","",20);

    // BuscarOrAnd b = buscarorand_crear(c, 50);
    // avl_recorrer(arbol, 0, contacto_buscar_and, b);
    // if (array_nelems(b->resultado)) {
    //     for (unsigned i = 0; i < b->resultado->numElems; i++) {
    //     contacto_imprimir(array_buscar(b->resultado, i), NULL);
    //     printf("\n");
    //     }
    // } else
    //       printf("No existe el subconjunto");

    // printf("\n\n-------------------------------\n\n");

    // AVL arbol_datos = cargar_contactos_avl("datos.csv");
    // avl_recorrer(arbol_datos, 0, contacto_imprimir, NULL);
    // guardar_contactos_avl(arbol_datos, "output.csv");
    //SHINE
    // contacto_destruir(c);
    // buscarorand_destruir(b);
    // avl_destruir(arbol_datos);
    // array_destruir(subConj);
    // array_destruir(array);
    // avl_destruir(arbol_edades);
    AVL arbol = avl_crear(contacto_copia, contacto_comparar_nombre, contacto_destruir);
    Lista lista = lista_crear(MAX_ACCIONES, accion_copia, accion_destruir);
    int accion;
    menu();
    //cargar_contactos_avl(arbol, "datos.csv");
    //eliminar_contactos_avl(arbol, "datos.csv");
    printf("Seleccione una accion:\n>");
    scanf("%d", &accion);
    getchar();
    while (accion != 13) {
        while (accion < 1 || accion > 13) {
            printf("Accion invalida, ingrese otra:\n>");
            scanf("%d", &accion);
        }
        accion_redirigir(accion, arbol, lista);
        //imprimir_lista_nodo(lista->prm);
        printf("Seleccione una accion:\n>");
        scanf("%d", &accion);
        getchar();
    }
    avl_destruir(arbol);
    lista_destruir(lista);
    return 0;
}
