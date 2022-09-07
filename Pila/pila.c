#include "pila.h"
#include <stdio.h>
#include <stdlib.h>
#define DEFAULT_SIZE 40
#define DEFAULT_UPSIZE 2
#define DEFAULT_DOWNSIZE 0.5
#define DEFAULT_DOWNSIZE_CONDITION_FACTOR 4
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

// Redimensiona la pila con el factor. Si falla, devuelve false.
// Post: devuelve una pila redimensionada.
bool pila_redimensionar(pila_t *pila, double factor);


/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
    pila_t *nueva_pila = malloc(sizeof(pila_t));
    if (nueva_pila==NULL) {
        return NULL;
    }
    nueva_pila->datos=malloc(DEFAULT_SIZE*sizeof(void*));
    if(nueva_pila->datos==NULL){
        return NULL;
    }
    nueva_pila->capacidad=DEFAULT_SIZE;
    nueva_pila->cantidad=0;
    return nueva_pila;
}


void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return (pila->cantidad==0);
}

bool pila_apilar(pila_t *pila, void *valor){
    if(pila->capacidad==pila->cantidad){
        if(!pila_redimensionar(pila,DEFAULT_UPSIZE)){
            return false;
        }
    }
    pila->datos[pila->cantidad]=valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    return (pila_esta_vacia(pila)) ? NULL : pila->datos[pila->cantidad-1];
}


void *pila_desapilar(pila_t *pila){
    if(pila_esta_vacia(pila)) {
        return NULL;
    }
    pila->cantidad--;
    if(pila->cantidad==pila->capacidad/DEFAULT_DOWNSIZE_CONDITION_FACTOR){
        pila_redimensionar(pila,DEFAULT_DOWNSIZE);
    }
    return pila->datos[pila->cantidad];
}


bool pila_redimensionar(pila_t *pila, double factor){
    void **aux;
    if(factor==0){
        return false;
    }
    aux=realloc(pila->datos,(unsigned long)((double)(pila->capacidad*sizeof(void*))*factor));
    if(aux==NULL){
        return false;
    }
    pila->capacidad=(unsigned long)(factor*(double)pila->capacidad);
    pila->datos=aux;
    return true;
}
