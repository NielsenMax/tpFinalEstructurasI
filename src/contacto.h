#ifndef __CONTACTO_H__
#define __CONTACTO_H__
#include "array.h"

/**
 *Un contacto tendra un 3 string, uno para el nombre, otro para el apellido y
 *el ultimo para el telefono. Tambien tendra un natural para la edad.
*/
struct _Contacto {
    char *nombre;
    char *apellido;
    unsigned edad;
    char *telefono;
};
typedef struct _Contacto *Contacto;

/**
 *Estructura con un Contacto y un Array que sera usada para poder pasarle
 *a las funciones de buscar or y and para que tengan el contacto a buscar
 *y un array donde insertar los que cumplan las condiciones.
*/
struct _BuscarOrAnd {
    Contacto dato;
    Array resultado;
};

typedef struct _BuscarOrAnd *BuscarOrAnd;

//Crea un contacto con los datos correspondientes
void* contacto_crear(char*, char*, char*, unsigned int);

//Crea otro contacto que sera una copia del pasado
void* contacto_copia(void*);

//Compara dos contactos por el nombre y luego el appellido
int contacto_comparar_nombre(void*, void*);

//Compara dos contactos por la edad, luego por el nombre y apellido
int contacto_comparar_edad(void*, void*);

//Compara dos contactos por el apellido y luego por el nombre
int contacto_comparar_apellido(void*, void*);

//Compara dos contactos por el telefono, luego por el nombre y apellido
int contacto_comparar_telefono(void*, void*);

//Libera la memoria de un contacto
void contacto_destruir(void*);

//Imprime en pantalla un contacto
void contacto_imprimir(void*, void*);

//Agrega a un arbol AVL el contacto pasado
void contacto_agregar_avl(void*, void*);

//Agrega a un Array el contacto pasado
void contacto_agregar_array(void*, void*);

//Crea una estructura BuscarOrAnd
BuscarOrAnd buscarorand_crear(Contacto, unsigned);

//Libera la memoria de una estructura BuscarOrAnd
void buscarorand_destruir(BuscarOrAnd);

//Agrega al array de resultado el contacto pasado si conincide con al menos
//un campo del contacto a buscar
void contacto_buscar_or(void*, void*);

//Agrega al array de resultado el contacto pasado si conincide con todos los
//campos no nulosdel contacto a buscar
void contacto_buscar_and(void*, void*);

//Calcula un subconjunto del array pasado que la suma de sus edades sea igual
//al natural pasado
Array suma_edades(Array, unsigned);

#endif /* __CONTACTO_H__ */
