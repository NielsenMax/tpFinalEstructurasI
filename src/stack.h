#ifndef __STACK_H__
#define __STACK_H__

typedef void *(*FuncionCopiadora)(void *dato);
/** Retorna una copia fisica del dato */
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef void (*FuncionDestructora)(void *dato);
/** Libera la memoria alocada para el dato */

typedef struct _Stack *Stack;

Stack stack_crear(FuncionCopiadora, FuncionDestructora);

void stack_destruir(Stack);

Stack stack_insertar(Stack, void*);

void* stack_top(Stack);
#endif
