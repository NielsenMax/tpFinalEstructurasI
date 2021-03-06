#include "acciones.h"
#include "contacto.h"
#include "avl.h"
#include "archivo.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_ACCIONES 20         //Maximo de la lista de deshacer/rehacer.

/**
 *menu :: void
 *imprime un menu con los numeros correspondientes a cada accion.
*/
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
  AVL arbol = avl_crear(contacto_copia, contacto_comparar_nombre,
                        contacto_destruir);
  Lista lista = lista_crear(MAX_ACCIONES, accion_copia, accion_destruir);
  int accion;
  menu();
  printf("Seleccione una accion:\n>");
  scanf("%d", &accion);
  getchar();
  while (accion != 13) {
    while (accion < 1 || accion > 13) {
      printf("Accion invalida, ingrese otra:\n>");
      scanf("%d", &accion);
    }
    accion_redirigir(accion, arbol, lista);
    printf("\nSeleccione una accion:\n>");
    scanf("%d", &accion);
    getchar();
  }
  avl_destruir(arbol);
  lista_destruir(lista);
  return 0;
}
