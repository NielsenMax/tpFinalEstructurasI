CFLAGS = -Wall -Wextra -Werror -std=c99 -g
CC = gcc

valgrind_test: test
	valgrind --tool=memcheck --leak-check=full ./test

test: tests.o acciones.o contacto.o array.o avl.o lista.o archivo.o
	$(CC) $(CFLAGS) -o test tests.o acciones.o contacto.o array.o avl.o lista.o archivo.o -lm

tests.o: ./tests/tests.c acciones.o contacto.o array.o avl.o lista.o archivo.o
	$(CC) $(CFLAGS) -c ./tests/tests.c -lm

main: main.o acciones.o contacto.o array.o avl.o lista.o archivo.o
	$(CC) $(CFLAGS) -o main main.o acciones.o contacto.o array.o avl.o lista.o archivo.o -lm

valgrind: main
	valgrind --tool=memcheck --leak-check=full ./main

main.o: ./src/main.c acciones.o contacto.o array.o avl.o lista.o archivo.o
	$(CC) $(CFLAGS) -c ./src/main.c -lm

acciones.o: ./src/acciones.h ./src/acciones.c contacto.o array.o avl.o lista.o archivo.o
	$(CC) $(CFLAGS) -c ./src/acciones.c -lm

archivo.o: ./src/archivo.h ./src/archivo.c contacto.o avl.o
	$(CC) $(CFLAGS) -c ./src/archivo.c -lm

contacto.o: ./src/contacto.h ./src/contacto.c array.o avl.o
	$(CC) $(CFLAGS) -c ./src/contacto.c -lm

array.o: ./src/array.h ./src/array.c
	$(CC) $(CFLAGS) -c ./src/array.c

lista.o: ./src/lista.h ./src/lista.c
	$(CC) $(CFLAGS) -c ./src/lista.c

avl.o: ./src/avl.h ./src/avl.c
	$(CC) $(CFLAGS) -c ./src/avl.c -lm
