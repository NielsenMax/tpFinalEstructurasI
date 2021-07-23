#include "avl.h"
#include "contacto.h"
#include "array.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *contacto_crear :: char* -> char* -> char* -> unsigned -> void*
 *Toma un string que representara el nombre del contacto, un string que sera
 *el apellido, un string que sera el telefono y un natural que sera la edad. Con
 *ellos generara un contacto y lo devolvera como un puntero a void.
*/
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
  assert(nuevo->telefono);
  strcpy(nuevo->telefono, telefono);

  nuevo->edad = edad;

  return nuevo;
}

/**
 *contacto_copia :: void* -> void*
 *Toma un puntero a void que representara un contacto y devolvera otro con
 *una copia del contacto, es decir con los mismos datos.
*/
void* contacto_copia(void* contacto) {
    Contacto copia = (Contacto) contacto;
      copia = contacto_crear(copia->nombre, copia->apellido,copia->telefono, copia->edad);
  return copia;
}

/**
 *contacto_comparar_nombre :: void* -> void* -> int
 *Toma dos punteros a void que representaran dos contactos, los comparara
 *devolvera un entero segun el resultado. Si el primero es menor dara un
 *negativo, si es mayor dara un positivo y dara 0 si son iguales.
 *Para compararlos primero comparara el nombre y luego el apellido.
*/
int contacto_comparar_nombre(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = strcmp(c1->nombre, c2->nombre);
    return ret ? ret : strcmp(c1->apellido, c2->apellido);
}

/**
 *contacto_comparar_edad :: void* -> void* -> int
 *Toma dos punteros a void que representaran dos contactos, los comparara
 *devolvera un entero segun el resultado. Si el primero es menor dara un
 *negativo, si es mayor dara un positivo y dara 0 si son iguales.
 *Compara por edad y si son iguales por el nombre y apellido.
 *Esto causa que la funcion buscar del avl no se comporte como deberia
 *ya que retornaria el primero que encuentre con la edad deseada.
 *No nos importa ya que no usaremos la funcion buscar sobre un arbol que sea
 *generado con esta funcion de comparar.
*/
int contacto_comparar_edad(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = (int)(c1->edad - c2->edad);
    return ret ? ret : contacto_comparar_nombre(c1, c2);
}

/**
 *contacto_comparar_apellido :: void* -> void* -> int
 *Toma dos punteros a void que representaran dos contactos, los comparara
 *devolvera un entero segun el resultado. Si el primero es menor dara un
 *negativo, si es mayor dara un positivo y dara 0 si son iguales.
 *Para compararlos primero comparara el apellido y luego el nombre.
*/
int contacto_comparar_apellido(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = strcmp(c1->apellido, c2->apellido);
    return ret ? ret : strcmp(c1->nombre, c2->nombre);
}

/**
 *contacto_comparar_telefono :: void* -> void* -> int
 *Toma dos punteros a void que representaran dos contactos, los comparara
 *devolvera un entero segun el resultado. Si el primero es menor dara un
 *negativo, si es mayor dara un positivo y dara 0 si son iguales.
 *Compara por el telefono y si son iguales por el nombre y apellido.
 *Esto causa que la funcion buscar del avl no se comporte como deberia
 *ya que retornaria el primero que encuentre con la edad deseada.
 *No nos importa ya que no usaremos la funcion buscar sobre un arbol que sea
 *generado con esta funcion de comparar.
*/
int contacto_comparar_telefono(void* con1, void* con2) {
    Contacto c1 = (Contacto) con1, c2 = (Contacto) con2;
    int ret = strcmp(c1->telefono, c2->telefono);
    return ret ? ret : contacto_comparar_nombre(c1, c2);
}

/**
 *contacto_destruir :: void* -> void
 *Toma un puntero a void que representara un contacto y liberara su memoria.
*/
void contacto_destruir(void* c) {
    Contacto contacto = (Contacto) c;
  free(contacto->nombre);
  free(contacto->apellido);
  free(contacto->telefono);
  free(c);
}

/**
 *contacto_imprimir :: void* -> void* -> void
 *Toma un puntero a void que represntara un contacto y otro que seran
 *parametros extras para la funcion, en este caso deben ser NULL.
 *Imprimira en pantalla los del contacto pasado.
*/
void contacto_imprimir(void* c, void* extras) {
    if (!extras) {
        Contacto contacto = (Contacto) c;
        printf("{%s, %s, %s, %d}", contacto->nombre, contacto->apellido, contacto->telefono, contacto->edad);
    }
}

/**
 *contacto_agregar_avl :: void* -> void* -> void
 *Toma un puntero a void que represntara un contacto y otro que seran
 *parametros extras para la funcion, en este caso sera un arbol AVL.
 *La funcion insertara el contacto en el arbol pasado.
*/
void contacto_agregar_avl(void* c, void* extras) {
    Contacto contacto = (Contacto) c;
    AVL arbol = (AVL) extras;
    avl_insertar(arbol, contacto);
}

/**
 *contacto_agregar_array :: void* -> void* -> void
 *Toma un puntero a void que represntara un contacto y otro que seran
 *parametros extras para la funcion, en este caso sera un Array.
 *La funcion insertara el contacto en el Array pasado.
*/
void contacto_agregar_array(void* c, void* extras) {
    Contacto contacto = (Contacto) c;
    Array array = (Array) extras;
    array_insertar(array, contacto);
}

/**
 *buscarorand_crear :: Contacto -> unsigned -> BuscarOrAnd
 *Toma un contacto y un natural, devolvera una estructura BuscarOrAnd con
 *el dato pasado como el para buscar y un array vacio de la capacidad pasada.
*/
BuscarOrAnd buscarorand_crear(Contacto c, unsigned n) {
    BuscarOrAnd buscar = malloc(sizeof(struct _BuscarOrAnd));
    assert(buscar);
    buscar->dato = contacto_copia(c);
    buscar->resultado = array_crear(n, contacto_copia, contacto_destruir);
    return buscar;
}

/**
 *buscarorand_destruir :: BuscarOrAnd -> void
 *Toma una estructura BuscarOrAnd y libera la memoria de la misma.
*/
void buscarorand_destruir(BuscarOrAnd b) {
    contacto_destruir(b->dato);
    array_destruir(b->resultado);
    free(b);
}

/**
 *contacto_buscar_or :: void* -> void* -> void
 *Toma un puntero a void que represntara un contacto y otro que seran
 *parametros extras para la funcion, en este caso sera una estructura
 *BuscarOrAnd.
 *La funcion insertara en el resultado de la estructura los contactos pasados
 *que coincidan en al menos un parametro con el contacto a buscar de la
 *estuctura. Los parametros del contacto a buscar pueden ser cadenas vacias o
 *cero para la edad, pero al menos uno debe no ser "nulo".
*/
void contacto_buscar_or(void* c, void* extras) {
    BuscarOrAnd buscar = (BuscarOrAnd) extras;
    Contacto contacto = (Contacto) c;
    int nombre = 0, apellido = 0, telefono = 0, edad = 0;
    if (!strcmp(buscar->dato->nombre, contacto->nombre))
        nombre++;
    if (!strcmp(buscar->dato->apellido, contacto->apellido))
        apellido++;
    if (!strcmp(buscar->dato->telefono, contacto->telefono))
        telefono++;
    if (buscar->dato->edad == contacto->edad)
        edad++;
    if (nombre || apellido || telefono || edad)
        array_insertar(buscar->resultado, contacto);
}

/**
 *contacto_buscar_and :: void* -> void* -> void
 *Toma un puntero a void que represntara un contacto y otro que seran
 *parametros extras para la funcion, en este caso sera una estructura
 *BuscarOrAnd.
 *La funcion insertara en el resultado de la estructura los contactos pasados
 *que coincidan en todos los parametros con el contacto a buscar de la
 *estuctura. Los parametros del contacto a buscar pueden ser cadenas vacias o
 *cero para la edad, pero al menos uno debe no ser "nulo".
*/
void contacto_buscar_and(void* c, void* extras) {
    BuscarOrAnd buscar = (BuscarOrAnd) extras;
    Contacto contacto = (Contacto) c;
    int nombre = 0, apellido = 0, telefono = 0, edad = 0;
    if (!strcmp(buscar->dato->nombre, "") || (!strcmp(buscar->dato->nombre, contacto->nombre)))
        nombre++;
    if (!strcmp(buscar->dato->apellido, "") || (!strcmp(buscar->dato->apellido, contacto->apellido)))
        apellido++;
    if (!strcmp(buscar->dato->telefono, "") || (!strcmp(buscar->dato->telefono, contacto->telefono)))
        telefono++;
    if (!buscar->dato->edad || buscar->dato->edad == contacto->edad)
        edad++;
    if (nombre && apellido && telefono && edad)
        array_insertar(buscar->resultado, contacto);
}

/**
 *suma_edades :: Array -> unsigned -> Array
 *Toma un array de contactos y un natural que sera la edad total a buscar.
 *Luego devolvera un array de contactos con los contactos que la suma de sus
 *edades den el numero pasado, si no existe estara vacio.
 *Para encontrar el subconjunto la funcion primero creara una matriz de
 *n * m, con n la cantidad de elementos del array pasado y m la edad total a
 *buscar. Luego la primer columna seran todos True(1) y la primer fila, sin
 *incluir a la primer casilla, seran todos False(0).
 *Para calcular el subconjunto la funcion recorrera la matriz desde la esquina
 *superior izquierda a la esquina inferior derecha asignando los valores que
 *correspondan a cada casilla segun el siguiente pseudocodigo:
 *si (A[i - 1] > j)
 *  DP[i][j] = DP[i - 1][j]
 *sino
 *  DP[i][j] = DP[i - 1][j] or DP[i - 1][j - A[i - 1]]
 *La idea general es que para generar el numero j con el elemento A[i - 1]
 *se debe poder generar el numero (j - A[i - 1]). Iniciando con el valor 0
 *como verdadero.
 *Si el subconjunto existe la esquina inferior derecha de la matriz tendra un
 *True(1) y se podra reconstruir al subconjunto simplemente si seguimos la
 *siguiente idea:
 *- Si la casilla donde estamos es True y la de arriba es False entonces el
 *  elemento A[i - 1] debe ser insertado y nos moveremos a la casilla
 *  DP[i - 1][j - A[i - 1]]
 *- Si la casilla donde estamos es True y la de arriba tambien nos moveremos
 *  a la casilla de arriba.
 *- Si llegamos a la primer columna terminamos.
*/
Array suma_edades(Array array, unsigned sumaTotal) {
    int dp[array_nelems(array) + 1][sumaTotal + 1];
    for (unsigned i = 0; i <= array_nelems(array); i++) //primer columna
        dp[i][0] = 1;
    for (unsigned j = 1; j <= sumaTotal; j++) //primer fila
        dp[0][j] = 0;
    for (unsigned i = 1; i <= array_nelems(array); i++) {
        for (unsigned j = 1; j <= sumaTotal; j++) {
            //Si el elemento correspondiente al array es mayor que el numero
            //que queremos formar ponemos el valor de arriba nuestro.
            if (((Contacto) array_buscar(array, i - 1))->edad > j)
                dp[i][j] = dp[i - 1][j];
            //Si el valor es menor nos fijamos si podiamos generar el valor
            //j - A[i-1]. Si no copiamos el de arriba.
            else
                dp[i][j] = dp[i - 1][j] || dp[i - 1][j - ((Contacto)array_buscar(array, i - 1))->edad];
        }
    }
    Array subConj = array_crear(array_nelems(array), contacto_copia, contacto_destruir);
    //Reconstruimos el subconjunto
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
/*
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
*/
