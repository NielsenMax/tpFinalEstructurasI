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

/**
 *accion_crear :: Contacto -> char* -> AccionTipo -> void*
 *Toma un contacto, un string y un natural de AccionTipo.
 *Luego crea una accion con el contacto como el pasado, el archivo como el
 *string pasado y el tipo como el natural pasado. Luego devuelve la accion
 *como un puntero a void.
*/
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

/**
 *accion_copia :: void* -> void*
 *Toma un puntero a void que representara una accion y crea una copia de la
 *misma y la devuelve.
*/
void* accion_copia(void* a) {
    Accion accion = (Accion) a;
    return accion_crear(accion->contacto, accion->archivo, accion->tipo);
}

/**
 *accion_destruir :: void* -> void
 *Toma un puntero a void que representara una accion y libera la memoria de la
 *misma.
*/
void accion_destruir(void* a) {
    if (a) {
        Accion accion = (Accion) a;
        free(accion->archivo);
        if (accion->contacto)
            contacto_destruir(accion->contacto);
        free(a);
    }
}

// void accion_imprimir(void* a) {
//     Accion accion = (Accion) a;
//     printf("{%i, %s, ", accion->tipo, accion->archivo);
//     if (accion->contacto)
//         contacto_imprimir(accion->contacto, NULL);
//     printf("}\n");
// }

/**
 *leer :: char*
 *Lee los caracteres ingresados por teclado hasta que se ingrese un salto de
 *linea, luego los devuelve como un string.
*/
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

/**
 *normalizar_string :: char* -> void
 *Toma un string y lo normaliza, es decir elimina todos los espacios extras.
 *Ademas elimina todos los espacios al principio y al final.
*/
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
  if (k > 0 && string[k - 1] == ' ')     //Elimina los espacios finales
    string[--k] = '\0';
  else
    string[k] = '\0';
}

/**
 *string_minusculas :: char* -> void
 *Toma un string y lo vuelve todos sus caracteres minusculas.
*/
void string_minusculas(char *string) {
    for (; *string; string++)
        *string = tolower(*string);
}

/**
 *contiene_coma :: char* -> int
 *Toma un string y devuelve 1 si contiene una coma, 0 sino.
*/
int contiene_coma(char *string) {
    for (;*string; string++) {
        if (*string == ',')
            return 1;
    }
    return 0;
}

/**
 *str_vacio :: char* -> int
 *Toma un string y comprueba si es igual al string "". Si lo es devuelve 1,
 *sino 0;
*/
int str_vacio(char* c) {return !strcmp(c, "");}

/**
 *accion_buscar :: AVL -> void
 *Funcion estatica que toma un arbol AVL. La funcion pide por teclado
 *un nombre y un apellido e imprime en pantalla el resto de datos que
 *encuentre en el arbol bajo ese nombre y apellido.
*/
static void accion_buscar(AVL arbol) {
    char* nombre = NULL, *apellido = NULL;
    while (!nombre) { //checkeo de input
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
        if (r)
            printf("Nombre: %s\nApellido: %s\nEdad: %d\nTelefono: %s\n", r->nombre, r->apellido, r->edad, r->telefono);
        else
            printf("No existe un contacto con ese nombre y apellido\n");
        contacto_destruir(c);
    free(nombre);
    free(apellido);
}

/**
 *accion_agregar :: AVL -> Lista -> void
 *Funcion estatica que toma un arbol AVL y una lista de acciones.
 *Luego pide que se ingrese los datos de un contacto por
 *teclado e inserta un contacto creado con esos datos en el arbol AVL.
 *Ademas inserta una accion del tipo AGREGAR, con el contacto igual al
 *insertado en el arbol y el archivo igual a "", en la lista pasada.
*/
static void accion_agregar(AVL arbol, Lista lista) {
    char* nombre = NULL, *apellido = NULL, *telefono = NULL;
    unsigned edad = 0;
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
    while (!edad) {
        printf("Ingrese la edad:\n>");
        scanf("%i", &edad);
        getchar();
        if (!edad) {
            printf("La edad no puede ser cero\n");
        }
    }
    while (!telefono) {
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

/**
 *accion_eliminar :: AVL -> Lista -> void
 *Funcion estatica que toma un arbol AVL y una lista de acciones.
 *Luego pide por teclado el nombre y apellido de un contacto
 *y lo elimina del arbol AVL. Si el contacto estaba en el arbol, entonces
 *crea una accion del tipo ELIMINAR, con el contacto igual al eliminado y
 *el archivo igual a "", y la inserta en la Lista.
*/
static void accion_eliminar(AVL arbol, Lista lista) {
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

/**
 *accion_editar :: AVL -> Lista -> void
 *Funcion estatica que toma un arbol y una lista de acciones.
 *Pide que se ingrese un nombre y un apellido de un contacto por teclado
 *y si este pertenece al arbol AVL pide la edad y el telefono para actualizar
 *el contacto del arbol. Si el contacto estaba en el arbol tambien crea una
 *accion del tipo EDITAR, con el contacto igual al anterior de la edicion y
 *el archivo igual a "", y lo agrega a la lista pasada.
*/
static void accion_editar(AVL arbol, Lista lista) {
    char* nombre = NULL, *apellido = NULL;
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
    Contacto c = contacto_crear(nombre, apellido, "", 0);
    Contacto r = avl_buscar(arbol, c);
    if (r) {
        unsigned edad = 0;
        char* telefono = NULL;
        Accion a = accion_crear(r, "", EDITAR);
        lista_insertar(lista, a);
        accion_destruir(a);
        printf("\nEl contacto contiene los siguientes datos:\n");
        printf("Nombre: %s\nApellido: %s\nEdad: %d\nTelefono: %s\n", r->nombre, r->apellido, r->edad, r->telefono);
        //printf("\nIngrese la nueva edad:\n>");

        while (!edad) {
        printf("Ingrese la nueva edad:\n>");
        scanf("%i", &edad);
        getchar();
        if (!edad)
            printf("La edad no puede ser cero\n");
        }
        while (!telefono) {
            printf("Ingrese el nuevo telefono:\n");
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
        r->edad = edad;
        strcpy(r->telefono, telefono);
        free(telefono);
        } else
            printf("No existe un contacto con ese nombre y apellido\n");
        contacto_destruir(c);
        free(nombre);
        free(apellido);
}

/**
 *accion_cargar :: AVL -> Lista -> void
 *Funcion estatica que toma un arbol AVL y una lista de acciones.
 *Luego pide que se ingrese la ruta de un archivo CSV con contactos y los
 *inserta en el arbol. Tambien crea una accion de tipo CARGAR, con un contacto
 *nulo y la ruta de archivo igual a la ingresada, que inserta en la lista.
*/
static void accion_cargar(AVL arbol, Lista lista) {
    char* input = NULL;
    while (!input) {
        printf("Ingrese el archivo de entrada:\n>");
        input = leer();
        if (!input) {
            printf("El ingreso no es valido\n");
        } else if (access(input, F_OK)) {
            printf("La ruta ingresada no existe o no el programa no puede acceder a ella\n");
            free(input);
            input = NULL;
        }
    }
    cargar_contactos_avl(arbol, input);
    Accion a = accion_crear(NULL, input, CARGAR);
    lista_insertar(lista, a);
    accion_destruir(a);
    free(input);
}

/**
 *accion_guardar :: AVL -> void
 *Funcion estatica que toma un arbol AVL.
 *Luego pide que se ingrese la ruta de un archivo CSV y escribe en este
 *todos los contactos que contenga el arbol. El arbol se recorre en inorden.
*/
static void accion_guardar(AVL arbol) {
    char* input = NULL;
    while (!input) {
        printf("Ingrese el archivo de salida:\n>");
        input = leer();
        if (!input) {
            printf("El ingreso no es valido\n");
        }
    }
        guardar_contactos_avl(arbol, input);
        free(input);
}

/**
 *accion_and_or :: AVL -> unsigned -> void
 *Funcion estatica que toma un arbol AVL y un natural entre 0 y 1.
 *Luego pide que se ingrese un nombre, un apellido, una edad y un telefono.
 *Alguno de estos pueden ser nulos o 0 (si es la edad) pero no todos.
 *Luego si el natural pasado es 0 realizara una busqueda de todos los contactos
 *del arbol que cumplan con todos las condiciones no nulas ingresadas.
 *Si es 1 buscara los contactos del arbol que cumplan al menos una.
*/
static void accion_and_or(AVL arbol, unsigned t) {
    char* nombre = NULL, *apellido = NULL, *telefono = NULL;
    unsigned edad = 0;
    while (!nombre) {
        printf("Ingrese un nombre (oprima enter si no quiere buscar por nombre):\n>");
        nombre = leer();
        normalizar_string(nombre);
        if (contiene_coma(nombre)) {
            printf("El nombre no puede contener comas.\n");
            free(nombre);
            nombre = NULL;
        } else if (!nombre)
            printf("El dato es invalido\n");
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
    }
        printf("Ingrese la edad (si no quiere buscar por edad ingrese 0):\n>");
        scanf("%u", &edad);
        getchar();
    while (!telefono) {
        printf("Ingrese el telefono:\n");
        telefono = leer();
        normalizar_string(telefono);
        if (contiene_coma(telefono)) {
            printf("El telefono no puede contener comas.\n");
            free(telefono);
            telefono = NULL;
        } else if (!telefono)
            printf("El dato es invalido\n");
    }
    if (str_vacio(nombre) && str_vacio(apellido) && str_vacio(telefono) && !edad) {
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

/**
 *accion_guardar_ordenado :: AVL -> void
 *Funcion estatica que toma un arbol AVL.
 *Luego pide que se ingrese el nombre de un archivo CSV y un atributo de
 *contacto (nombre, apellido, edad o telefono). Luego guarda los contactos
 *del arbol ordenados por el atributo ingresado de forma creciente.
*/
static void accion_guardar_ordenado(AVL arbol) {
    char* input = NULL;
    while (!input) {
        printf("Ingrese el archivo de salida:\n>");
        input = leer();
        if (!input) {
            printf("El ingreso no es valido\n");
        }
    }
    char* atributo = NULL;
    while (!atributo) {
        printf("Ingrese el campo a ordenar por:\n>");
        atributo = leer();
        normalizar_string(atributo);
        string_minusculas(atributo);
        if (!atributo) {
            printf("El ingreso no es valido\n");
        } else if (strcmp("nombre", atributo) && strcmp("apellido", atributo) && strcmp("telefono", atributo) && strcmp("edad", atributo)) {
            printf("El atributo solo puede ser nombre, apellido, telefono o edad\n");
            free(atributo);
            atributo = NULL;
        }
    }
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
        free(input);
    }

/**
 *accion_buscar_suma_edades :: AVL -> void
 *Funcion estatica que toma un arbol AVL.
 *Luego pide un natural y la funcion mostrara un subconjunto de contactos
 *del arbol que la suma de sus edades sea igual al natural ingresado.
*/
static void accion_buscar_suma_edades (AVL arbol) {
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

/**
 *accion_deshacer :: AVL -> Lista -> void
 *Funcion estatica que toma un arbol AVL y una lista de acciones.
 *Luego deshace la ultima accion que figure en la lista segun el tipo que sea:
 *-AGREGAR: elimina el contacto agregado del arbol.
 *-ELIMINAR: agrega el contacto eliminado al arbol.
 *-EDITAR: pone los datos originales del dato.
 *-CARGAR: elimina del arbol todos los contactos agregados.
*/
static void accion_deshacer(AVL arbol, Lista lista) {
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
            Contacto aux = contacto_copia(c); //guarda los datos editados
            c->edad = deshacer->contacto->edad;
            strcpy(c->telefono, deshacer->contacto->telefono);
            //actualiza los datos de la lista a los editados
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

/**
 *accion_rehacer :: AVL -> Lista -> void
 *Funcion estatica que toma un arbol AVL y una lista de acciones.
 *Si la lista tiene alguna accion para rehacer la rehace.
 *Es decir si tiene un nodo siguiente al apuntado por el puntero de
 *accion actual.
*/
static void accion_rehacer(AVL arbol, Lista lista) {
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
            Contacto aux = contacto_copia(c);
            c->edad = rehacer->contacto->edad;
            strcpy(c->telefono, rehacer->contacto->telefono);
            //almacena en la lista los datos anteriores a la edicion
            rehacer->contacto->edad = aux->edad;
            strcpy(rehacer->contacto->telefono, aux->telefono);
            contacto_destruir(aux);
            break;
        case CARGAR:
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

/**
 *accion_redirigir :: AccionTipo -> AVL -> Lista -> void
 *Toma un natural del tipo AccionTipo, un arbol AVL y una lista de acciones.
 *Luego ejecuta la accion necesaria segun el valor del natural.
*/
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
