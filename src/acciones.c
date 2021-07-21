#include "acciones.h"
#include "avl.h"
#include "lista.h"
#include "archivo.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

void* accion_crear(Contacto c, char* archivo, AccionTipo n) {
    Accion nueva = malloc(sizeof(struct _Contacto));
    assert(nueva);
    nueva->archivo = malloc(sizeof(char) * (strlen(archivo) + 1));
    assert(nueva->archivo);
    strcpy(nueva->archivo, archivo);
    if (c)
        nueva->contacto = contacto_copia(c);
    else
        nueva->contacto = NULL;
    nueva->tipo = n;
    return nueva;
}

void* accion_copia(void* a) {
    Accion accion = (Accion) a;
    return accion_crear(accion->contacto, accion->archivo, accion->tipo);
}

void accion_destruir(void* a) {
    if (a) {
        Accion accion = (Accion) a;
        free(accion->archivo);
        if (accion->contacto)
            contacto_destruir(accion->contacto);
        free(a);
    }
}

void accion_imprimir(void* a) {
    Accion accion = (Accion) a;
    printf("{%i, %s, ", accion->tipo, accion->archivo);
    if (accion->contacto)
        contacto_imprimir(accion->contacto, NULL);
    printf("}\n");
}

char *leer() {
  char *line = NULL, *tmp = NULL;
  size_t size = 0, index = 0;
  int ch = EOF;
  while (ch) {
    ch = getchar();             //Cargamos un caracter por teclado
    if (ch == EOF || ch == '\n')        //Si es un '\n' lo colocamos en 0
      ch = 0;
    if (size <= index) {
      //Si el tamaño que usamos es mas chico que el largo de la input
      size += 15;               //Sumamos al tamaño
      tmp = realloc(line, size);        //Realocamos memoria con el nuevo tamaño
      if (!tmp) {
        //Si tmp es NULL luego de realocar liberamos la memoria
        free(line);
        line = NULL;
        break;
      }
      line = tmp;
    }
    line[index++] = ch;         //Añadimos el caracter
  }
  return line;
}

void normalizar_string(char *string) {
  int space = 0;                //1 si se encuentra un espacio 0 si es cualquier otro caracter
  int k = 0;                    //apunta a la siguiente posicion libre
  for (int i = 0; string[i]; i++) {
//Elimina los espacios al inicio
    while (k == 0 && string[i] && string[i] == ' ')
      i++;
    if (string[i] == ' ') {
      if (!space)               //Venimos de un caracter y encontramos espacio
      {
//Guardamos el espacio y marccamos la flag
        string[k++] = string[i];
        space = 1;
      }
    } else {
//Guardamos el caraccter si no es un espacio y marcamos la flag
      string[k++] = string[i];
      space = 0;
    }
  }
  if (string[k - 1] == ' ')     //Elimina los espacios finales
    string[--k] = '\0';
  else
    string[k] = '\0';
  for (; *string; string++)
      *string = tolower(*string);
}

int contiene_coma(char *string) {
    for (;*string; string++) {
        if (*string == ',')
            return 1;
    }
    return 0;
}

void accion_buscar(AVL arbol) {
    char* nombre = NULL, *apellido = NULL;
    while (!nombre) {
        printf("Ingrese un nombre:\n>");
        nombre = leer();
        normalizar_string(nombre);
        if (contiene_coma(nombre)) {
            printf("El nombre no puede contener comas.\n");
            free(nombre);
            nombre = NULL;
        }else if (!nombre)
            printf("El dato es invalido\n");
        else if (str_vacio(nombre))
            printf("El dato no puede estar vacio\n");
    }
    while (!apellido) {
        printf("Ingrese un apellido:\n>");
        apellido = leer();
        normalizar_string(apellido);
        if (contiene_coma(apellido)) {
            printf("El apellido no puede contener comas.\n");
            free(apellido);
            apellido = NULL;
        }else  if (!apellido)
            printf("El dato es invalido\n");
        else if (str_vacio(apellido))
            printf("El dato no puede estar vacio\n");
    }
    if (!nombre || !apellido)
        printf("Uno de los datos no es valido");
    else {
        Contacto c = contacto_crear(nombre, apellido, "", 0);
        Contacto r = avl_buscar(arbol, c);
        if (r)
            printf("Nombre: %s\nApellido: %s\nEdad: %d\nTelefono: %s\n", r->nombre, r->apellido, r->edad, r->telefono);
        else
            printf("No existe un contacto con ese nombre y apellido\n");
        contacto_destruir(c);
    }
    free(nombre);
    free(apellido);
}

int str_vacio(char* c) {return !strcmp(c, "");}

void accion_agregar(AVL arbol, Lista lista) {
    char* nombre = NULL, *apellido = NULL, *telefono = NULL;
    int edad = -1;
    while (!nombre) {
        printf("Ingrese un nombre:\n>");
        nombre = leer();
        normalizar_string(nombre);
        if (contiene_coma(nombre)) {
            printf("El nombre no puede contener comas.\n");
            free(nombre);
            nombre = NULL;
        } else if (!nombre)
            printf("El dato es invalido\n");
        else if (str_vacio(nombre))
            printf("El dato no puede estar vacio\n");
    }
    while (!apellido) {
        printf("Ingrese un apellido:\n>");
        apellido = leer();
        normalizar_string(apellido);
        if (contiene_coma(apellido)) {
            printf("El apellido no puede contener comas.\n");
            free(apellido);
            apellido = NULL;
        } else if (!apellido)
            printf("El dato es invalido\n");
        else if (str_vacio(apellido))
            printf("El dato no puede estar vacio\n");
    }
    while (edad < 0) {
        printf("Ingrese la edad:\n>");
        scanf("%i", &edad);
        getchar();
        if (edad < 0) {
            printf("La edad no puede ser negativo\n");
        }
    }
    while (!nombre) {
        printf("Ingrese el telefono:\n");
        telefono = leer();
        normalizar_string(telefono);
        if (contiene_coma(telefono)) {
            printf("El telefono no puede contener comas.\n");
            free(telefono);
            telefono = NULL;
        } else if (!telefono)
            printf("El dato es invalido\n");
        else if (str_vacio(telefono))
            printf("El dato no puede estar vacio\n");
    }
    Contacto c = contacto_crear(nombre, apellido, telefono, edad);
    avl_insertar(arbol, c);
    Accion a = accion_crear(c, "", AGREGAR);
    lista_insertar(lista, a);
    contacto_destruir(c);
    accion_destruir(a);
    free(nombre);
    free(apellido);
    free(telefono);
}

void accion_eliminar(AVL arbol, Lista lista) {
    char* nombre = NULL, *apellido = NULL;
    while (!nombre) {
        printf("Ingrese un nombre:\n>");
        nombre = leer();
        normalizar_string(nombre);
        if (contiene_coma(nombre)) {
            printf("El nombre no puede contener comas.\n");
            free(nombre);
            nombre = NULL;
        }else if (!nombre)
            printf("El dato es invalido\n");
        else if (str_vacio(nombre))
            printf("El dato no puede estar vacio\n");
    }
    while (!apellido) {
        printf("Ingrese un apellido:\n>");
        apellido = leer();
        normalizar_string(apellido);
        if (contiene_coma(apellido)) {
            printf("El apellido no puede contener comas.\n");
            free(apellido);
            apellido = NULL;
        }else  if (!apellido)
            printf("El dato es invalido\n");
        else if (str_vacio(apellido))
            printf("El dato no puede estar vacio\n");
    }
    Contacto c = contacto_crear(nombre, apellido, "", 0);
    Contacto r = avl_buscar(arbol, c);
    if (r) {
        Accion a = accion_crear(r, "", ELIMINAR);
        lista_insertar(lista, a);
        avl_eliminar(arbol, c);
        accion_destruir(a);
    }
    contacto_destruir(c);
    free(nombre);
    free(apellido);
}

void accion_editar(AVL arbol, Lista lista) {
    char* nombre, *apellido;
    printf("Ingrese un nombre:\n>");
    nombre = leer();
    printf("Ingrese un apellido:\n>");
    apellido = leer();
    if (!nombre || !apellido)
        printf("Uno de los datos no es valido\n");
    else {
        Contacto c = contacto_crear(nombre, apellido, "", 0);
        Contacto r = avl_buscar(arbol, c);
        if (r) {
            unsigned edad;
            char* telefono;
            Accion a = accion_crear(r, "", EDITAR);
            lista_insertar(lista, a);
            accion_destruir(a);
            printf("El contacto contiene los siguientes datos:\n");
            printf("Nombre: %s\nApellido: %s\nEdad: %d\nTelefono: %s\n", r->nombre, r->apellido, r->edad, r->telefono);
            printf("Ingrese la nueva edad:\n>");
            scanf("%u", &edad);
            getchar();
            printf("Ingrese el nuevo telefono:\n>");
            telefono = leer();
            r->edad = edad;
            strcpy(r->telefono, telefono);
            free(telefono);
        } else
            printf("No existe un contacto con ese nombre y apellido\n");
        contacto_destruir(c);
        free(nombre);
        free(apellido);
    }
}

void accion_cargar(AVL arbol, Lista lista) {
    printf("Ingrese el archivo de entrada:\n>");
    char* input = leer();
    //printf("nombreee: %s", input);
    //FILE* archivo = fopen(input, "r");
    if (!input)
        printf("Ingreso invalido");
    else {
        if (!access(input, F_OK)) {
            //printf("%s", input);
            cargar_contactos_avl(arbol, input);
            Accion a = accion_crear(NULL, input, CARGAR);
            lista_insertar(lista, a);
            accion_destruir(a);
        } else
            printf("No existe un archivo con ese nombre\n");
        free(input);
    }
}

void accion_guardar(AVL arbol) {
    printf("Ingrese ruta de salida:\n>");
    char* input = leer();
    //FILE* archivo = fopen(input, "r");
    if (!input)
        printf("Ingreso invalido");
    else {
        guardar_contactos_avl(arbol, input);
        free(input);
    }
}

void accion_and_or(AVL arbol, unsigned t) {
    char* nombre, *apellido, *telefono;
    unsigned edad = 0;
    printf("Ingrese el nombre:\n>");
    nombre = leer();
    printf("Ingrese el apellido:\n>");
    apellido = leer();
    printf("Ingrese la edad(si no quiere buscar por edad ingrese 0):\n>");
    scanf("%u", &edad);
    getchar();
    printf("Ingrese el telefono:\n");
    telefono = leer();
    if (!nombre || !apellido || !telefono) {
        printf("Uno de los datos no es valido\n");
    } else if (str_vacio(nombre) && str_vacio(apellido) && str_vacio(telefono) && !edad) {
        printf("Al menos uno de los parametros tiene que no ser nulo\n");
    } else {
            Contacto c = contacto_crear(nombre, apellido, telefono, edad);
            BuscarOrAnd b = buscarorand_crear(c, avl_numero_nodos(arbol));
            if (t)
                avl_recorrer(arbol, 0, contacto_buscar_or, b);
            else
                avl_recorrer(arbol, 0, contacto_buscar_and, b);
            if (array_nelems(b->resultado)) {
                for (unsigned i = 0; i < b->resultado->numElems; i++) {
                    contacto_imprimir(array_buscar(b->resultado, i), NULL);
                    printf("\n");
                }
            } else
                printf("No existen contactos con esas caracteristicas\n");
            contacto_destruir(c);
            buscarorand_destruir(b);
    }
    free(nombre);
    free(apellido);
    free(telefono);
}

void accion_guardar_ordenado(AVL arbol) {
    printf("Ingrese ruta de salida:\n>");
    char* input = leer();
    //FILE* archivo = fopen(input, "r");
    if (!input)
        printf("Ingreso invalido");
    else {
        printf("Ingrese nombre de atributo:\n>");
        char* atributo = leer();
        if (!atributo) {
            printf("Ingreso invalido");
        } else {
            if (!strcmp("nombre", atributo)) {
                guardar_contactos_avl(arbol, input);
            } else if (!strcmp("apellido", atributo)) {
                AVL arbol_apellido = avl_crear(contacto_copia, contacto_comparar_apellido, contacto_destruir);
                avl_recorrer(arbol, 0, contacto_agregar_avl, arbol_apellido);
                guardar_contactos_avl(arbol_apellido, input);
                avl_destruir(arbol_apellido);
            } else if (!strcmp("telefono", atributo)) {
                AVL arbol_tel = avl_crear(contacto_copia, contacto_comparar_telefono, contacto_destruir);
                avl_recorrer(arbol, 0, contacto_agregar_avl, arbol_tel);
                guardar_contactos_avl(arbol_tel, input);
                avl_destruir(arbol_tel);
            } else if (!strcmp("edad", atributo)) {
                AVL arbol_edades = avl_crear(contacto_copia, contacto_comparar_edad, contacto_destruir);
                avl_recorrer(arbol, 0, contacto_agregar_avl, arbol_edades);
                guardar_contactos_avl(arbol_edades, input);
                avl_destruir(arbol_edades);
            } else {
                printf("No es un atributo valido\n");
            }
            free(atributo);
        }
        free(input);
    }
}

void accion_buscar_suma_edades (AVL arbol) {
    Array array = array_crear(avl_numero_nodos(arbol), contacto_copia, contacto_destruir);
    avl_recorrer(arbol, 0, contacto_agregar_array, array);
    printf("Ingrese un natural:\n>");
    unsigned edad;
    scanf("%u", &edad);
    getchar();
    Array subConj = suma_edades(array, edad);
    if (array_nelems(subConj)) {
        for (unsigned i = 0; i < subConj->numElems; i++) {
        contacto_imprimir(array_buscar(subConj, i), NULL);
        printf("\n");
        }
    } else
        printf("No existe un subconjunto que la suma de su edad resulte en %d\n", edad);
    array_destruir(array);
    array_destruir(subConj);
}

void accion_deshacer(AVL arbol, Lista lista) {
    Accion deshacer = lista_anterior(lista);
    if (deshacer) {
        switch (deshacer->tipo) {
        case AGREGAR:
            avl_eliminar(arbol, deshacer->contacto);
        break;
        case ELIMINAR:
            avl_insertar(arbol, deshacer->contacto);
            break;
        case EDITAR:;
            Contacto c = avl_buscar(arbol, deshacer->contacto);
            //avl_insertar(arbol, rehacer->contacto);
            Contacto aux = contacto_copia(c);
            c->edad = deshacer->contacto->edad;
            strcpy(c->telefono, deshacer->contacto->telefono);
            deshacer->contacto->edad = aux->edad;
            strcpy(deshacer->contacto->telefono, aux->telefono);
            contacto_destruir(aux);
            break;
        case CARGAR:
            if (!access(deshacer->archivo, F_OK))
                eliminar_contactos_avl(arbol, deshacer->archivo);
            break;
        default:
            break;
        }
    }
}

void accion_rehacer(AVL arbol, Lista lista) {
    Accion rehacer = lista_siguiente(lista);
    if (rehacer) {
        switch (rehacer->tipo) {
            case AGREGAR:
            avl_insertar(arbol, rehacer->contacto);
        break;
        case ELIMINAR:
            avl_eliminar(arbol, rehacer->contacto);
            break;
        case EDITAR:;
            Contacto c = avl_buscar(arbol, rehacer->contacto);
            //avl_insertar(arbol, rehacer->contacto);
            Contacto aux = contacto_copia(c);
            c->edad = rehacer->contacto->edad;
            strcpy(c->telefono, rehacer->contacto->telefono);
            rehacer->contacto->edad = aux->edad;
            strcpy(rehacer->contacto->telefono, aux->telefono);
            contacto_destruir(aux);
            break;
        case CARGAR:
            //printf("%s", rehacer->archivo);
            if (!access(rehacer->archivo, F_OK))
                cargar_contactos_avl(arbol, rehacer->archivo);
            break;
        default:
            break;
        }
    } else {
        printf("No hay nada que rehacer\n");
    }
}

void accion_redirigir(AccionTipo a, AVL arbol, Lista lista) {
    switch (a) {
    case BUSCAR:
        accion_buscar(arbol);
        break;
    case AGREGAR:
        accion_agregar(arbol, lista);
        break;
    case ELIMINAR:
        accion_eliminar(arbol, lista);
        break;
    case EDITAR:
        accion_editar(arbol, lista);
        break;
    case CARGAR:
        accion_cargar(arbol, lista);
        break;
    case GUARDAR:
        accion_guardar(arbol);
        break;
    case DESHACER:
        accion_deshacer(arbol, lista);
        break;
    case REHACER:
        accion_rehacer(arbol, lista);
        break;
    case AND:
        accion_and_or(arbol, 0);
        break;
    case OR:
        accion_and_or(arbol, 1);
        break;
    case GUARDAR_ORDENADO:
        accion_guardar_ordenado(arbol);
        break;
    case BUSCAR_SUMA_EDADES:
        accion_buscar_suma_edades(arbol);
        break;
    case SALIR:
        break;
    }
}
