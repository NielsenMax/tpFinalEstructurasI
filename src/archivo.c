#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contacto.h"
#include "avl.h"

/**
 *avl_insertar_linea :: AVL -> char* -> void
 *Funcion estatica que toma un arbol AVL y un string que corresponde
 *a un contacto en formato CSV. Insertara el contacto en el arbol AVL.
*/
static void avl_insertar_linea(AVL arbol, char* buffer) {
    char* nombre, *apellido, *telefono = NULL;
    unsigned edad;
    nombre = strsep(&buffer, ",");
    apellido = strsep(&buffer, ",");
    edad = atoi(strsep(&buffer, ","));
    telefono = strsep(&buffer, "\n");
    if (nombre && apellido && edad && telefono) {
        Contacto c = contacto_crear(nombre, apellido, telefono, edad);
        avl_insertar(arbol, c);
        contacto_destruir(c);
    }
}

/**
 *cargar_contactos_avl :: AVL -> char* -> void
 *Toma un arbol AVL y un string que representara un archivo CSV de contactos.
 *Luego insertara los contactos del archivo en el arbol.
*/
void cargar_contactos_avl(AVL arbol, char* a) {
    //printf("HOLAAAA%s",  a);
    FILE* archivo = fopen(a, "r");
    if (archivo) {
    //AVL arbol = avl_crear(contacto_copia, contacto_comparar_nombre, contacto_destruir);
    char* buffer = NULL, *linea = NULL;
    int nombre, apellido, edad, telefono;
    size_t bufferT = 0;
    ssize_t lineaT;
    lineaT = getline(&buffer, &bufferT, archivo);
    //printf("AHHHHHHH");
    if (lineaT != -1) {
        linea = buffer; //evita memory leaks
        nombre = strcmp(strsep(&buffer, ","), "nombre");
        apellido = strcmp(strsep(&buffer, ","), "apellido");
        edad = strcmp(strsep(&buffer, ","), "edad");
        telefono = strcmp(strsep(&buffer, "\n"), "telefono");
        if (nombre || apellido || edad || telefono) {
            free(linea);
            fclose(archivo);
        } else {
            free(linea);
            buffer = NULL;
        lineaT = getline(&buffer, &bufferT, archivo);
        linea = buffer;
        while (lineaT >= 0) {
            //printf("%s", buffer);
            // if (lineaT != -1)
                avl_insertar_linea(arbol, buffer);
            free(linea);
            buffer = NULL;
            lineaT = getline(&buffer, &bufferT, archivo);
            linea = buffer;
        }
        free(linea);
    }
    }
    if (archivo)
        fclose(archivo);
    }
}

/**
 *escribir_contacto_avl :: void* -> void* -> void
 *Funcion estatica que toma un puntero a void que represntara un contacto
 *y otro que sera un puntero FILE. Luego escribira el contacto en formato
 *CSV en el archivo.
*/
static void escribir_contacto_avl(void* c, void* f) {
    FILE* archivo = (FILE*) f;
    Contacto contacto = (Contacto) c;
    fprintf(archivo, "%s,%s,%d,%s\n", contacto->nombre, contacto->apellido, contacto->edad, contacto->telefono);
}

/**
 *guardar_contactos_avl :: AVL -> char* -> void
 *Toma un arbol AVL y un string que sera la ruta de un archivo.
 *Luego escribira en el archivo todos los contactos del arbol en formato CSV
 *recorriendolo de forma inorden.
*/
void guardar_contactos_avl(AVL arbol, char* c) {
    FILE* archivo = fopen(c, "w");
    if (archivo) {
        fprintf(archivo, "nombre,apellido,edad,telefono\n");
        avl_recorrer(arbol, 0, escribir_contacto_avl, archivo);
    }
    fclose(archivo);
}
