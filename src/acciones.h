#ifndef __ACCIONES_H__
#define __ACCIONES_H__

#include "contacto.h"
#include "lista.h"
#include "avl.h"

//Valores numericos para los tipos de acciones
typedef enum {
    SALIR,
    BUSCAR,
    AGREGAR,
    ELIMINAR,
    EDITAR,
    CARGAR,
    GUARDAR,
    DESHACER,
    REHACER,
    AND,
    OR,
    GUARDAR_ORDENADO,
    BUSCAR_SUMA_EDADES
} AccionTipo;

/**
 *Una accion constara de un contacto, un string que sera la ruta a un
 *archivo CSV y un natural entre 0 y 12 que sera el tipo de accion a
 *realizar.
*/
struct _Accion {
    Contacto contacto;
    char* archivo;
    AccionTipo tipo;
};

typedef struct _Accion *Accion;

//Crea una accion apartir de los datos pasados.
void* accion_crear(Contacto, char*, AccionTipo);

//Crea una copia de una accion apartir de otra
void* accion_copia(void*);

//Libera la memoria de una accion
void accion_destruir(void*);

//void accion_imprimir(void*);

//Lee la entrada por teclado hasta llegar a un salto de linea y la devuelve.
char *leer();

//Redirije las acciones segun el natural pasado para que se
//ejecuten correctamente
void accion_redirigir(AccionTipo, AVL, Lista);

#endif
