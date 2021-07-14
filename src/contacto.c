#include "avl.h"
#include "contacto.h"
#include "array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* contacto_crear(char* nombre, char* apellido, char* telefono, unsigned int edad) {
  Contacto nuevo = malloc(sizeof(struct _Contacto));
  assert(nuevo != NULL);

  nuevo->nombre = malloc(sizeof(char) * (strlen(nombre) + 1));
  assert(nuevo->nombre != NULL);
  strcpy(nuevo->nombre, nombre);

  nuevo->apellido = malloc(sizeof(char) * (strlen(apellido) + 1));
  assert(nuevo->apellido != NULL);
  strcpy(nuevo->apellido, apellido);

  nuevo->telefono = malloc(sizeof(char) * (strlen(telefono) + 1));
  strcpy(nuevo->telefono, telefono);

  nuevo->edad = edad;

  return nuevo;
}

void* contacto_copia(void* contacto) {
    Contacto copia = (Contacto) contacto;
      copia = contacto_crear(copia->nombre, copia->apellido,copia->telefono, copia->edad);
  return copia;
}

int contacto_comparar_nombre(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = strcmp(c1->nombre, c2->nombre);
    return ret ? ret : strcmp(c1->apellido, c2->apellido);
}

//Compara por edad y si son iguales por el nombre y apellido.
//Esto causa que la funcion buscar del avl no se comporte como deberia
//ya que retornaria el primero que encuentre con la edad deseada.
//No nos importa ya que no usaremos la funcion buscar sobre ese arbol.
int contacto_comparar_edad(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = (int)(c1->edad - c2->edad);
    return ret ? ret : contacto_comparar_nombre(c1, c2);
}

int contacto_comparar_apellido(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = strcmp(c1->apellido, c2->apellido);
    return ret ? ret : strcmp(c1->nombre, c2->nombre);
}

int contacto_comparar_telefono(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = strcmp(c1->telefono, c2->telefono);
    return ret ? ret : contacto_comparar_nombre(c1, c2);
}

void contacto_destruir(void* c) {
    Contacto contacto = (Contacto) c;
  free(contacto->nombre);
  free(contacto->apellido);
  free(contacto->telefono);
  //free(contacto);
  free(c);
}

void contacto_imprimir(void* c, void* extras) {
    Contacto contacto = (Contacto) c;
    printf("{%s, %s, %s, %d}", contacto->nombre, contacto->apellido, contacto->telefono, contacto->edad);
}

void contacto_agregar_avl(void* c, void* extras) {
    Contacto contacto = (Contacto) c;
    AVL arbol = (AVL) extras;
    avl_insertar(arbol, contacto);
}

void contacto_agregar_array(void* c, void* extras) {
    Contacto contacto = (Contacto) c;
    Array array = (Array) extras;
    array_insertar(array, contacto);
}

Array suma_edades(Array array, unsigned sumaTotal) {
    int dp[array_nelems(array) + 1][sumaTotal + 1];
    for (unsigned i = 0; i <= array_nelems(array); i++)
        dp[i][0] = 1;
    for (unsigned j = 1; j <= sumaTotal; j++)
        dp[0][j] = 0;
    for (unsigned i = 1; i <= array_nelems(array); i++) {
        for (unsigned j = 1; j <= sumaTotal; j++) {
            // printf("%i %i\n", ((Contacto) array_buscar(array, i - 1))->edad, j);
            if (((Contacto) array_buscar(array, i - 1))->edad > j)
                dp[i][j] = dp[i - 1][j];
            else
                dp[i][j] = dp[i - 1][j] || dp[i - 1][j - ((Contacto)array_buscar(array, i - 1))->edad];
            // printf("%i\n", dp[i][j]);
        }
    }
    // for (unsigned i = 0; i <= array_nelems(array); i++) {
    //     for (unsigned j = 0; j <= sumaTotal; j++)
    //         printf("%i", dp[i][j]);
    //     printf("\n");
    // }
    // printf("HOLAAAA");
    Array subConj = array_crear(array_nelems(array), contacto_copia, contacto_destruir);
    if (dp[array_nelems(array)][sumaTotal]) {
        unsigned i = array_nelems(array), j = sumaTotal;
        while (j > 0 && i > 0) {
            if (!dp[i - 1][j]) {
                array_insertar(subConj, array_buscar(array, i - 1));
                j -= ((Contacto) array_buscar(array, i-1))->edad;
            }
            i--;
        }
    }
    return subConj;
}

int main(){
    AVL arbol = avl_crear(contacto_copia, contacto_comparar_nombre, contacto_destruir);
    Contacto c = contacto_crear("maxi", "nielsen", "3413190720",20);
    avl_insertar(arbol, c);
    contacto_destruir(c);
     c = contacto_crear("alejo", "gonzalez", "3413190720",25);
    avl_insertar(arbol, c);
    contacto_destruir(c);
    c = contacto_crear("alejo", "a", "3413190720",21);
    avl_insertar(arbol, c);
    contacto_destruir(c);
    c = contacto_crear("z", "a", "3413190720",19);
    avl_insertar(arbol, c);
    contacto_destruir(c);
    c = contacto_crear("z", "b", "3413190720",30);
    avl_insertar(arbol, c);
    contacto_destruir(c);
    c = contacto_crear("z","a", "", 0);
    avl_eliminar(arbol,c);
    contacto_destruir(c);
    avl_recorrer(arbol, 0, contacto_imprimir, NULL);
    printf("\n");

    AVL arbol_edades = avl_crear(contacto_copia, contacto_comparar_edad, contacto_destruir);
    avl_recorrer(arbol, 0, contacto_agregar_avl, arbol_edades);
    avl_recorrer(arbol_edades, 0, contacto_imprimir, NULL);
    printf("\n");
    Array array = array_crear(50, contacto_copia, contacto_destruir);
    // c = contacto_crear("z", "b", "3413190720",30);
    //  array_insertar(array, c);
    //  array_insertar(array, c);
    //  contacto_imprimir(array_buscar(array, 1), NULL);
    // contacto_destruir(c);

    avl_recorrer(arbol, 0, contacto_agregar_array, array);
    for (unsigned i = 0; i < array->numElems; i++) {
        contacto_imprimir(array_buscar(array, i), NULL);
        printf("\n");
    }
    Array subConj = suma_edades(array, 0);
    if (array_nelems(subConj)) {
        for (unsigned i = 0; i < subConj->numElems; i++) {
        contacto_imprimir(array_buscar(subConj, i), NULL);
        printf("\n");
        }
    } else
          printf("No existe el subconjunto");
    array_destruir(subConj);
    array_destruir(array);
    avl_destruir(arbol_edades);
    avl_destruir(arbol);
    return 0;
}
