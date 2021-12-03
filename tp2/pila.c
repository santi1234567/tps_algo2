#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#define TAM_INICIAL 20
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) return NULL;
    pila->cantidad = 0;
    pila->capacidad = TAM_INICIAL;
    pila->datos = malloc(sizeof(void*)*TAM_INICIAL);
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

static bool redimensionar(pila_t *pila, size_t tamanio){
    void **aux = realloc(pila->datos, sizeof(void*)*tamanio);
    if (aux == NULL) return false;
    pila->capacidad = tamanio;
    pila->datos = aux;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad)
        if (!redimensionar(pila, pila->capacidad*2)) return false;
    pila->datos[pila->cantidad] = valor;
    pila->cantidad ++;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    return pila->cantidad == 0?  NULL: pila->datos[pila->cantidad-1];
}

void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)) return NULL;
    pila->cantidad --;
    if (pila->cantidad*4 <= pila->capacidad && pila->capacidad > TAM_INICIAL) 
        redimensionar(pila, pila->capacidad/2);
    return pila->datos[pila->cantidad];
}