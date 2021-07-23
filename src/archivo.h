#ifndef __ARCHIVO_H__
#define __ARCHIVO_H__
#include "avl.h"

//Inserta los contactos de un archivo CSV a un arbol AVL
void cargar_contactos_avl(AVL, char*);

//Elimina los contactos de un archivo CSV de un arbol AVL
void eliminar_contactos_avl(AVL, char*);

//Escribe los contactos de un arbol AVL a un archivo CSV
void guardar_contactos_avl(AVL, char* c);

#endif
