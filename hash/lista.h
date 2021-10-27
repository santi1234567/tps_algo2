#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdbool.h>

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

// Primitivas lista

// Crea una lista
// Post: Devuelve la lista o NULL en caso de que falle
lista_t *lista_crear(void);

// Dice si la lista está vacía
// Pre: lista fue creada
// Post: devuelve true si está vacía, false si no lo está
bool lista_esta_vacia(const lista_t *lista);

// Inserta un elemento en el primer lugar de la lista
// Pre: la lista fue creada
// Post: se agrego el dato a un nodo en el primer lugar de la lista y devuelve true si todo salió bien, false si falló algo
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un elemento al final de la lista
// Pre: la lista fue creada
// Post: se agrego el dato a un nodo en el último lugar de la lista y devuelve true si todo salió bien, false si falló algo
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Borra el primer elemento de la lista
// Pre: la lista fue creada
// Post: se borró el primer elemento de la lista
void *lista_borrar_primero(lista_t *lista);

// Devuelve el dato que se encuentra en el primer nodo de la lista
// Pre: la lista fue creada
// Post: devuelve el dato almacenado en el primer nodo (si la lista está vacía devuelve NULL)
void *lista_ver_primero(const lista_t *lista);

// Devuelve el dato que se encuentra en el ultimo nodo de la lista
// Pre: la lista fue creada
// Post: devuelve el dato almacenado en el ultimo nodo (si la lista está vacía devuelve NULL)
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista
// Pre: la lista fue creada
// Post: devuelve la cantidad de elementos en la lista (0 si está vacía)
size_t lista_largo(const lista_t *lista);

// Destruye la lista
// Pre: la lista y función de destrucción fueron creadas
// Post: destruye la lista
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Itera la lista mientras la función visitar devuelva true. Cuando esta devuelve false,
// indica que debe detenerse la iteración. La variable extra apunta a un parámetro que puede ser
// utilizado en la función visitar.
// Pre: la lista fue creada.
// Post: se itera la lista aplicando la función ingresada como parámetro
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


// Primitivas Iterador

// Crea un iterador externo
// Pre: La lista fue creada
// Post: Devuelve un nuevo iterador externo o NULL en caso de que falle
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza al siguiente elemento de la lista
// Pre: El iterador fue creado
// Post: Devuelve true si se pudo avanzar correctamente y false si se llegó al final
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el dato del elemento sobre el que está parado el iterador o NULL si está al final
// Pre: El iterador fue creado
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador se encuentra al final de la lista, false en el caso contrario.
// Pre: El iterador fue creado
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el Iterador.
// Pre: El iterador fue creado
// Post: El iterador fue destruido
void lista_iter_destruir(lista_iter_t *iter);

// Inserta el dato en la lista en la posición del iterador. Devuelve true si la operación
// se realizó correctamente y false si falló.
// Pre: El iterador fue creado
// Post: Se insertó en la lista el dato
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el dato de la lista en la posición del iterador.
// Pre: El iterador fue creado
// Post: Se eliminó de la lista el dato
void *lista_iter_borrar(lista_iter_t *iter);

#endif
