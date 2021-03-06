#ifndef __AVL_H__
#define __AVL_H__

/**
 *Toma un puntero a void que representara un dato y devuelve otro
 *que contenga la misma informacion
*/
typedef void *(*FuncionCopiadora)(void *dato);
/**
 *Toma dos punteros a void que representaran dos datos y los
 *comprada, devolviendo 0 si son iguales, un negativo si el
 *primero es menor y un positivo si el segundo es menor
*/
typedef int (*FuncionComparadora)(void *, void *);
/**
 *Toma un puntero a void que representara un dato y libera la memoria de este.
*/
typedef void (*FuncionDestructora)(void *dato);
/**
 *Toma un puntero a void que representara un dato y otro que seran extras
 *para que la funcion pueda realizar algo sobre este dato generalmente.
*/
typedef void (*FuncionVisitanteExtra)(void *dato, void *extra);

typedef enum {
  AVL_RECORRIDO_IN,             /* Inorden */
  AVL_RECORRIDO_PRE,            /* Preorden */
  AVL_RECORRIDO_POST            /* Postorden */
} AVLRecorrido;

//puntero a struct _AVL para facilitar su uso
typedef struct _AVL *AVL;

/**
 * Retorna un arbol AVL vacio
 */
AVL avl_crear(FuncionCopiadora, FuncionComparadora, FuncionDestructora);

/**
 * Destruye el arbol y sus datos.
 */
void avl_destruir(AVL);

/**
 * Retorna el dato completo si el pasado se encuentra en el arbol
 */
void *avl_buscar(AVL, void *);

/**
 *Devuelve la cantidad maxima de nodos que puede tener un arbol
 *sin cambiar la altura
*/
int avl_numero_nodos(AVL);

/**
 * Inserta un dato no repetido en el arbol, manteniendo la propiedad de los
 * arboles AVL.
 */
void avl_insertar(AVL, void *);

/**
 * Retorna 1 si el arbol cumple la propiedad de los arboles AVL, y 0 en caso
 * contrario.
 */
int avl_validar(AVL);

/**
*Elimina un dato del arbol si este se encuentre en el, mantiene la propiedad
*de los arboles AVL.
*/
void avl_eliminar(AVL, void *);

/**
 * Recorrido DSF del arbol
 */
void avl_recorrer(AVL, AVLRecorrido, FuncionVisitanteExtra, void *);

#endif                          /* __AVL_H__ */
