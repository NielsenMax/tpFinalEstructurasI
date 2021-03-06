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
 *Una accion constara de un contactoy un natural entre 0 y 12
 *que sera el tipo de accion a realizar. Para esta implementacion solo se
 *deberian usar del 2 al 4 ya que son las unicas que se pueden deshacer.
*/
struct _Accion {
  Contacto contacto;
  AccionTipo tipo;
};

typedef struct _Accion *Accion;

//Crea una accion apartir de los datos pasados.
void *accion_crear(Contacto, AccionTipo);

//Crea una copia de una accion apartir de otra
void *accion_copia(void *);

//Libera la memoria de una accion
void accion_destruir(void *);

//Lee la entrada por teclado hasta llegar a un salto de linea y la devuelve.
char *leer();

//Elimina los espacios demas de un string
void normalizar_string(char *);

//Pasa un string a minusculas
void string_minusculas(char *);

//Devuelve 1 si el string pasado contiene comas, sino 0
int contiene_coma(char *);

//Devuelve 1 si el string pasado es igual a "", sino 0
int str_vacio(char *);

//Redirije las acciones segun el natural pasado para que se
//ejecuten correctamente
void accion_redirigir(AccionTipo, AVL, Lista);

#endif
