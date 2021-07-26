#include "../src/avl.h"
#include "../src/lista.h"
#include "../src/array.h"
#include "../src/contacto.h"
#include "../src/acciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct _Auxiliar {
    int* numeros;
    int indice;
};
typedef struct _Auxiliar *Auxiliar;

static void* copiar_puntero_entero(void* i) {
  int* p = malloc(sizeof(int));
  *p = *(int*)i;
  return p;
}
static int comparar_puntero_entero(void* i1, void* i2) {
  return (*(int*)i1 - *(int*)i2);
}

static void destruir_puntero_entero(void* i) { free(i);}

static void insertar_puntero_entero_en_Auxiliar(void* i, void* extra) {
  Auxiliar numeros = (Auxiliar) extra;
  numeros->numeros[numeros->indice++] = *(int*) i;
}

void test_avl() {
    printf("TESTEANDO FUNCIONES DE avl.h\n");
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

static void test_lista() {
    printf("TESTEANDO FUNCIONES DE lista.h\n");
    {
        printf("TESTEANDO FUNCION lista_insertar\n");
        Lista lista = lista_crear(8, copiar_puntero_entero, destruir_puntero_entero);
        int numeros[] = {23, 56, 78, 21, 2, 35, 49, 64};
        for (int i = 0; i < 8; i++)
            lista_insertar(lista, numeros + i);
        assert(lista_largo(lista) == 8);
        for (int i = 0; i < 8; i++) {
            assert(*(int*)lista->act->dato == numeros[7 - i]);
            lista->act = lista->act->ant;
        }
        lista_destruir(lista);
        lista = lista_crear(5, copiar_puntero_entero, destruir_puntero_entero);
        int numeros2[] = {23, 56, 78, 21, 2, 35, 49, 64};
        for (int i = 0; i < 8; i++)
            lista_insertar(lista, numeros2 + i);
        //Libera los primeros 3
        assert(lista_largo(lista) == 5);
        assert(*(int*)lista->prm->dato == numeros2[3]);
        assert(*(int*)lista->act->dato == numeros2[7]);
        lista_destruir(lista);
    }
    {
        printf("TESTEANDO FUNCION lista_anterior\n");
        Lista lista = lista_crear(8, copiar_puntero_entero, destruir_puntero_entero);
        int numeros[] = {23, 56, 78, 21, 2, 35, 49, 64};
        for (int i = 0; i < 8; i++)
            lista_insertar(lista, numeros + i);
        for (int i = 0; i < 8; i++)
            assert(*(int*)lista_anterior(lista) == numeros[7 - i]);
        assert(!lista->act->dato);
//checkeamos que el hay un dato "nulo" ya que terminamos la lista
        assert(!lista->prm->dato);
        //No nos importa pasarnos de la capacidad ya que cuando insertemos
        //eliminaremos el nodo extra.
        assert(lista_largo(lista) == 9);
        printf("TESTEANDO FUNCION lista_siguiente\n");
        for (int i = 0; i < 8; i++)
            assert(*(int*)lista_siguiente(lista) == numeros[i]);
        assert(lista->prm->dato);
        assert(lista_largo(lista) == 8);
        lista_destruir(lista);

        lista = lista_crear(8, copiar_puntero_entero, destruir_puntero_entero);
        int numeros2[] = {23, 56, 78, 21, 2, 35, 49, 64};
        for (int i = 0; i < 8; i++)
            lista_insertar(lista, numeros2 + i);
        for (int i = 0; i < 5; i++)
            lista_anterior(lista);
        int a = 13;
        lista_insertar(lista, &a);
        //comprobamos que funcione la eliminacion de nodos a rehacer
        assert(lista_largo(lista) == 4);
        for (int i = 0; i < 4; i++)
            lista_anterior(lista);
        a = 5;
        lista_insertar(lista, &a);
        //comprobamos que funcione la eliminacion del nodo nulo
        assert(lista_largo(lista) == 1);
        lista_destruir(lista);
    }
}

static void test_array() {
    printf("TESTEANDO FUNCIONES DE array.h\n");
    {
        printf("TESTEANDO FUNCION array_insertar\n");
        Array array = array_crear(8, copiar_puntero_entero, destruir_puntero_entero);
        int numeros[] = {23, 56, 78, 21, 2, 35, 49, 64};
        for (int i = 0; i < 8; i++)
            array_insertar(array, numeros + i);
        assert(array_nelems(array) == 8);
        assert(array_capacidad(array) == 8);
        for (int i = 0; i < 8; i++)
            assert(*(int*)array->elems[i].dato == numeros[i]);
        array_destruir(array);
    }
    {
        printf("TESTEANDO FUNCION array_buscar\n");
        Array array =  array_crear(12, copiar_puntero_entero, destruir_puntero_entero);
        int numeros[] = {89, 556, 758, 121, 22, 3885, 449, 164, 42, 69, 33};
        for (int i = 0; i < 11; i++)
            array_insertar(array, numeros + i);
        assert(array_nelems(array) == 11);
        assert(array_capacidad(array) == 12);
        for (int i = 0; i < 8; i++)
            assert(*(int*)array_buscar(array, i) == numeros[i]);
        assert(!array_buscar(array, 11));//no asignado
        assert(!array_buscar(array, 13));//mayor a la capacidad
        array_destruir(array);
    }
}

static void test_contacto() {
    printf("TESTEANDO FUNCIONES DE contacto.h\n");
    {
        printf("TESTEANDO FUNCION contacto_comparar_nombre\n");
        Contacto a = contacto_crear("a","b","",0);
        Contacto b = contacto_crear("b","b","",0);
        assert(contacto_comparar_nombre(a,b) < 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","c", "", 0);
        b = contacto_crear("a","b", "", 0);
        assert(contacto_comparar_nombre(a,b) > 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","b", "", 0);
        b = contacto_crear("a","b", "", 0);
        assert(contacto_comparar_nombre(a,b) == 0);
        contacto_destruir(a);
        contacto_destruir(b);
    }
    {
        printf("TESTEANDO FUNCION contacto_comparar_edad\n");
        Contacto a = contacto_crear("a","b", "", 19);
        Contacto b = contacto_crear("b","b", "", 20);
        assert(contacto_comparar_edad(a,b) < 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","c", "", 10);
        b = contacto_crear("a","b", "", 10);
        assert(contacto_comparar_edad(a,b) > 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","b", "", 10);
        b = contacto_crear("a","b", "", 10);
        assert(contacto_comparar_edad(a,b) == 0);
        contacto_destruir(a);
        contacto_destruir(b);
    }
    {
        printf("TESTEANDO FUNCION contacto_comparar_apellido\n");
        Contacto a = contacto_crear("a","b", "", 0);
        Contacto b = contacto_crear("b","b", "", 0);
        assert(contacto_comparar_apellido(a,b) < 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","c", "", 0);
        b = contacto_crear("a","b", "", 0);
        assert(contacto_comparar_apellido(a,b) > 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","b", "", 0);
        b = contacto_crear("a","b", "", 0);
        assert(contacto_comparar_apellido(a,b) == 0);
        contacto_destruir(a);
        contacto_destruir(b);
    }
    {
        printf("TESTEANDO FUNCION contacto_comparar_telefono\n");
        Contacto a = contacto_crear("a","b", "1", 0);
        Contacto b = contacto_crear("b","b", "2", 0);
        assert(contacto_comparar_telefono(a,b) < 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","c", "1", 0);
        b = contacto_crear("a","b", "1",0);
        assert(contacto_comparar_telefono(a,b) > 0);
        contacto_destruir(a);
        contacto_destruir(b);
        a = contacto_crear("a","b", "1", 0);
        b = contacto_crear("a","b", "1", 0);
        assert(contacto_comparar_telefono(a,b) == 0);
        contacto_destruir(a);
        contacto_destruir(b);
    }
    {
        printf("TESTEANDO FUNCION contacto_agregar_avl\n");
        AVL arbol = avl_crear(contacto_copia, contacto_comparar_nombre, contacto_destruir);
        Contacto a = contacto_crear("a","b", "1", 10);
        contacto_agregar_avl(a, arbol);
        Contacto b = avl_buscar(arbol, a);
        //comparar por telefono y nombre compara todos los campos
        assert(contacto_comparar_edad(a, b) == 0 && contacto_comparar_telefono(a, b) == 0);
        contacto_destruir(a);
        //contacto_destruir(b);
        a = contacto_crear("aaa","bbb", "123456789", 10);
        contacto_agregar_avl(a, arbol);
        b = avl_buscar(arbol, a);
        assert(contacto_comparar_edad(a, b) == 0 && contacto_comparar_telefono(a, b) == 0);
        contacto_destruir(a);
        //contacto_destruir(b);
        avl_destruir(arbol);
    }
    {
        printf("TESTEANDO FUNCION contacto_agregar_array\n");
        Array array = array_crear(2, contacto_copia, contacto_destruir);
        Contacto a = (Contacto)contacto_crear("a","b", "1", 10);
        contacto_agregar_array(a, array);
        Contacto b = (Contacto)array_buscar(array, 0);
        assert(contacto_comparar_edad(a, b) == 0 && contacto_comparar_telefono(a, b) == 0);
        contacto_destruir(a);
        //contacto_destruir(b);
        a = contacto_crear("aaa","bbb", "123456789", 10);
        contacto_agregar_array(a, array);
        b = array_buscar(array, 1);
        assert(contacto_comparar_edad(a, b) == 0 && contacto_comparar_telefono(a, b) == 0);
        contacto_destruir(a);
        //contacto_destruir(b);
        array_destruir(array);
    }
    {
        printf("TESTEANDO FUNCION contacto_buscar_or\n");
        Contacto a = contacto_crear("a", "b", "123456", 10);
        BuscarOrAnd buscar = buscarorand_crear(a, 5);
        contacto_destruir(a);
        a = contacto_crear("a", "c", "123456", 9);
        contacto_buscar_or(a, buscar);
        Contacto b = (Contacto) array_buscar(buscar->resultado, 0);
        assert(!contacto_comparar_edad(b, a) && !contacto_comparar_telefono(b, a));
        contacto_destruir(a);
        //contacto_destruir(b);
        //Ningun parametro es igual
        a = contacto_crear("b", "c", "1234567", 9);
        contacto_buscar_or(a, buscar);
        //No agrego otro elemento
        assert(array_nelems(buscar->resultado) == 1);
        contacto_destruir(a);
        a = contacto_crear("a", "b", "123456", 10);
        contacto_buscar_or(a, buscar);
        b = (Contacto) array_buscar(buscar->resultado, 1);
        assert(!contacto_comparar_edad(b, a) && !contacto_comparar_telefono(b, a));
        contacto_destruir(a);
        //contacto_destruir(b);
        buscarorand_destruir(buscar);

        //contacto a buscar con menos parametros
        a = contacto_crear("a", "b", "", 0);
        buscar = buscarorand_crear(a, 5);
        contacto_destruir(a);
        a = contacto_crear("a", "c", "123456", 9);
        contacto_buscar_or(a, buscar);
        b = (Contacto) array_buscar(buscar->resultado, 0);
        assert(!contacto_comparar_edad(b, a) && !contacto_comparar_telefono(b, a));
        contacto_destruir(a);
        //contacto_destruir(b);
        buscarorand_destruir(buscar);
    }
    {
        printf("TESTEANDO FUNCION contacto_buscar_and\n");
        Contacto a = contacto_crear("a", "", "123456", 0);
        BuscarOrAnd buscar = buscarorand_crear(a, 5);
        contacto_destruir(a);
        a = contacto_crear("a", "c", "123456", 9);
        contacto_buscar_or(a, buscar);
        Contacto b = (Contacto) array_buscar(buscar->resultado, 0);
        assert(!contacto_comparar_edad(b, a) && !contacto_comparar_telefono(b, a));
        contacto_destruir(a);
        //contacto_destruir(b);
        //un solo parametro es igual
        a = contacto_crear("b", "c", "123456", 9);
        contacto_buscar_and(a, buscar);
        assert(array_nelems(buscar->resultado) == 1);
        contacto_destruir(a);
        a = contacto_crear("a", "b", "123456", 10);
        contacto_buscar_or(a, buscar);
        b = (Contacto) array_buscar(buscar->resultado, 1);
        assert(!contacto_comparar_edad(b, a) && !contacto_comparar_telefono(b, a));
        contacto_destruir(a);
        //contacto_destruir(b);
        buscarorand_destruir(buscar);
    }
    {
        printf("TESTEANDO FUNCION suma_edades\n");
        Array array = array_crear(6, contacto_copia, contacto_destruir);
        Contacto a = contacto_crear("a", "b", "1234", 19);
        array_insertar(array, a);
        contacto_destruir(a);
        a = contacto_crear("a", "b", "1234", 65);
        array_insertar(array, a);
        contacto_destruir(a);
        a = contacto_crear("a", "b", "1234", 8);
        array_insertar(array, a);
        contacto_destruir(a);
        a = contacto_crear("a", "b", "1234", 23);
        array_insertar(array, a);
        contacto_destruir(a);
        a = contacto_crear("a", "b", "1234", 75);
        array_insertar(array, a);
        contacto_destruir(a);
        a = contacto_crear("a", "b", "1234", 40);
        array_insertar(array, a);
        contacto_destruir(a);
        Array resultado = suma_edades(array, 27);
        assert(array_nelems(resultado) == 2);
        assert(((Contacto) array_buscar(resultado, 0))->edad == 8);
        assert(((Contacto) array_buscar(resultado, 1))->edad == 19);
        array_destruir(resultado);
        resultado = suma_edades(array, 25);
        assert(array_nelems(resultado) == 0);
        array_destruir(resultado);
        resultado = suma_edades(array, 113);
        assert(array_nelems(resultado) == 3);
        assert(((Contacto) array_buscar(resultado, 0))->edad == 40);
        assert(((Contacto) array_buscar(resultado, 1))->edad == 8);
        assert(((Contacto) array_buscar(resultado, 2))->edad == 65);
        array_destruir(resultado);
        array_destruir(array);
    }
}

static void test_acciones() {
    printf("TESTEANDO FUNCIONES DE acciones.h\n");
    {
        printf("TESTEANDO FUNCION normalizar_string\n");
        char a[] = "   hola  mundo  ";
        normalizar_string(a);
        assert(!strcmp(a, "hola mundo"));
        char b[] = " maxi   nielsen";
        normalizar_string(b);
        assert(!strcmp(b, "maxi nielsen"));
        char c[] = "la    casa linda    ";
        normalizar_string(c);
        assert(!strcmp(c, "la casa linda"));
    }
    {
        printf("TESTEANDO FUNCION string_minusculas\n");
        char a[] = "HoLa MunDo";
        string_minusculas(a);
        assert(!strcmp(a, "hola mundo"));
        char b[] = "123456789";
        string_minusculas(b);
        assert(!strcmp(b, "123456789"));
        char c[] = "hola mundo";
        string_minusculas(c);
        assert(!strcmp(c, "hola mundo"));
    }
    {
        printf("TESTEANDO FUNCION contiene_coma\n");
        assert(contiene_coma("hola, mundo"));
        assert(contiene_coma(",hola mundo"));
        assert(contiene_coma("hola mundo,"));
        assert(!contiene_coma("hola mundo"));
    }
    {
        printf("TESTEANDO FUNCION str_vacio\n");
        assert(!str_vacio("hola mundo"));
        assert(str_vacio(""));
        assert(!str_vacio("a"));
    }
}

int main() {
    test_avl();
    test_lista();
    test_array();
    test_contacto();
    test_acciones();
}
