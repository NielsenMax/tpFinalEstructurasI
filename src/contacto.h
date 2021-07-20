#ifndef __CONTACTO_H__
#define __CONTACTO_H__
#include "array.h"
struct _Contacto {
    char *nombre;
    char *apellido;
    unsigned edad;
    char *telefono;
};
typedef struct _Contacto *Contacto;

struct _BuscarOrAnd {
    Contacto dato;
    Array resultado;
};

typedef struct _BuscarOrAnd *BuscarOrAnd;

void* contacto_crear(char*, char*, char*, unsigned int);

void* contacto_copia(void*);

int contacto_comparar_nombre(void*, void*);

int contacto_comparar_edad(void*, void*);

int contacto_comparar_apellido(void*, void*);

int contacto_comparar_telefono(void*, void*);

void contacto_destruir(void*);

void contacto_imprimir(void*, void*);

void contacto_agregar_avl(void*, void*);

void contacto_agregar_array(void*, void*);

BuscarOrAnd buscarorand_crear(Contacto, unsigned);

void buscarorand_destruir(BuscarOrAnd);
void contacto_buscar_or(void*, void*);

void contacto_buscar_and(void*, void*);

Array suma_edades(Array, unsigned);

#endif /* __CONTACTO_H__ */
