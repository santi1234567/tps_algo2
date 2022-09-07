#include "cola.h"
#include <stdio.h>
#include <stdlib.h>

/* Definición del struct nodo */
typedef struct nodo nodo_t;

struct nodo {
    void* dato; // Apunta al dato almacenado
    nodo_t* prox; // Apunta al proximo nodo
};

// Crea un nodo. Si falla, devuelve NULL.
// Post: devuelve un nodo con el dato ingresado.
nodo_t *nodo_crear(void* dato);

/* Definición del struct cola */
struct cola {
    nodo_t* primero; // Apunta al primer Nodo de la Cola
    nodo_t* ultimo; // Apunta al ultimo Nodo de la Cola
};


nodo_t *nodo_crear(void* dato) {
    nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
    if (nuevo_nodo == NULL) {
        return NULL;
    }
    nuevo_nodo->dato = dato;
    nuevo_nodo->prox = NULL;
    return nuevo_nodo;
}


/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void){
    cola_t *nueva_cola = malloc(sizeof(cola_t));
    if (nueva_cola == NULL) {
        return NULL;
    }
    nueva_cola->primero = NULL;
    nueva_cola->ultimo = NULL;
    return nueva_cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
    while (!cola_esta_vacia(cola)) {
        void *aux = cola_desencolar(cola);
        if(destruir_dato != NULL) {
            destruir_dato(aux);
        }
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->ultimo==NULL;
}

bool cola_encolar(cola_t *cola, void *valor) {
    nodo_t *aux;
    aux = nodo_crear(valor);
    if (aux == NULL) {
        return false;
    }
    if(cola_esta_vacia(cola)) {
        cola->primero = aux;
    } else {
        cola->ultimo->prox = aux;
    }
    cola->ultimo = aux;
    return true;
}

void *cola_ver_primero(const cola_t *cola){
    return  (cola_esta_vacia(cola)) ? NULL : cola->primero->dato;
}

void *cola_desencolar(cola_t *cola){
    nodo_t *aux;
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    void *valor=cola_ver_primero(cola);
    aux=cola->primero;
    cola->primero=cola->primero->prox;
    if(cola->primero == NULL) {
        cola->ultimo = NULL;
    }
    free(aux);
    return valor;
}


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_cola.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en cola.h
// (esto es, las pruebas no pueden acceder a los miembros del struct cola).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_cola_estudiante(void);
