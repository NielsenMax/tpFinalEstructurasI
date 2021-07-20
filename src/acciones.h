#ifndef __ACCIONES_H__
#define __ACCIONES_H__

#include "contacto.h"
#include "lista.h"
#include "avl.h"

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

struct _Accion {
    Contacto contacto;
    char* archivo;
    AccionTipo tipo;
};

typedef struct _Accion *Accion;

void* accion_crear(Contacto, char*, AccionTipo);

void* accion_copia(void*);

void accion_destruir(void*);

void accion_imprimir(void*);

char *leer();

void accion_redirigir(AccionTipo, AVL, Lista);

#endif
