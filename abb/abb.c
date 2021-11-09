#include "abb.h"
#include "pila.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct abb_nodo {
    struct abb_nodo* izq;
	struct abb_nodo* der;
	void* dato;
	char* clave;
} abb_nodo_t;

struct abb {
	abb_nodo_t* raiz;
	abb_destruir_dato_t destruir_dato;
	abb_comparar_clave_t cmp;
};


struct abb_iter {
    pila_t *pila;
};

abb_nodo_t *abb_nodo_crear(const char *clave, void *dato) {
	abb_nodo_t *nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = malloc(sizeof(char)*(strlen(clave)+1));
	if (!nodo->clave) {
		free(nodo);
		return NULL;
	}
	strcpy(nodo->clave,clave);
	nodo->dato = dato;
	return nodo;
}

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t *arbol = malloc(sizeof(abb_t));
	if(!arbol) return NULL;
	arbol->destruir_dato = destruir_dato;
	arbol->cmp = cmp;
	arbol->raiz = NULL;
    return arbol;
}

static bool abb_guardar_rec(abb_nodo_t **nodo, const char *clave, void *dato, abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	if (!(*nodo)) {
		(*nodo) = abb_nodo_crear(clave, dato);
		return ((*nodo));
	}
	int comparacion = cmp(clave, (*nodo)->clave);
	if(comparacion == 0) {
		void *aux = (*nodo)->dato;
    (*nodo)->dato = dato;
    if(destruir_dato) destruir_dato(aux);
		return true;
	} else if (comparacion > 0) {
		return abb_guardar_rec(&((*nodo)->der), clave, dato, cmp, destruir_dato);
	}
	return abb_guardar_rec(&((*nodo)->izq), clave, dato, cmp, destruir_dato);
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	if (!arbol->raiz) {
		arbol->raiz = abb_nodo_crear(clave, dato);
		return true;
	}
	return abb_guardar_rec(&(arbol->raiz), clave, dato, arbol->cmp, arbol->destruir_dato);
}


static void *abb_borrar_rec(abb_nodo_t **padre, abb_nodo_t **nodo, const char *clave, abb_comparar_clave_t cmp) {
	if (!*nodo) return NULL;
	abb_nodo_t *hijo;
	if (!cmp(clave, (*nodo)->clave)) { // las claves son iguales
		if(!(*nodo)->izq || !(*nodo)->der) { // si el nodo tiene 0 o 1 hijos
			void *dato = (*nodo)->dato; // recupero dato

			if ((*nodo)->izq == NULL) { // veo en cual de los lados esta el hijo. en el caso de 0 hijos sirve para poner null
				hijo = (*nodo)->der;
			} else {
				hijo = (*nodo)->izq;
			}

			free((*nodo)->clave);
			if (padre) { // si no es la raiz (tiene padre)
				if ((*padre)->izq == (*nodo)) { // me fijo de que lado era hijo y elimino referencia
					free((*nodo));
					(*padre)->izq = hijo;
				} else {
					free((*nodo));
					(*padre)->der = hijo;
				}
			} else {
				free((*nodo));
				(*nodo) = hijo;
			}
			return dato;
		}
    if (!nodo) return NULL;
    abb_nodo_t *reemplazo_padre = *nodo;
    abb_nodo_t *reemplazo = (*nodo)->izq;
    while(reemplazo->der) {
        reemplazo_padre = reemplazo;
        reemplazo = reemplazo->der;
    }

    char *clave_reemplazo = malloc(sizeof(char)*(strlen(reemplazo->clave)+1));
    if (!clave_reemplazo) return NULL;
    strcpy(clave_reemplazo, reemplazo->clave);
    void *dato = abb_borrar_rec(&reemplazo_padre, &reemplazo, reemplazo->clave, cmp);
    free((*nodo)->clave);
    (*nodo)->clave = clave_reemplazo;
    void *aux = (*nodo)->dato;
    (*nodo)->dato = dato;
    return aux;

	} else if (cmp(clave, (*nodo)->clave) > 0) { // la clave es mayor a la del nodo
		return abb_borrar_rec(nodo, &(*nodo)->der, clave, cmp);
	} // la clave es menor a la del nodo
	return abb_borrar_rec(nodo, &(*nodo)->izq, clave, cmp);
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	return abb_borrar_rec(NULL, &arbol->raiz, clave, arbol->cmp);
}


static void *abb_obtener_rec(const abb_nodo_t *nodo, const char *clave, abb_comparar_clave_t cmp) {
	if (!nodo) return NULL;
	int comparacion = cmp(clave, nodo->clave);
	if(comparacion == 0) {
		return nodo->dato;
	} else if (comparacion > 0) {
		return abb_obtener_rec(nodo->der, clave, cmp);
	}
	return abb_obtener_rec(nodo->izq, clave, cmp);

}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	return (arbol->raiz) ? abb_obtener_rec(arbol->raiz, clave, arbol->cmp) : NULL;
}

static bool abb_pertenece_rec(abb_nodo_t *nodo, const char *clave, abb_comparar_clave_t cmp) {
	if (!nodo) return false;
	int comparacion = cmp(clave, nodo->clave);
	if(comparacion == 0) {
		return true;
	} else if (comparacion > 0) {
		return abb_pertenece_rec(nodo->der, clave, cmp);
	}
	return abb_pertenece_rec(nodo->izq, clave, cmp);
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	return (arbol->raiz) ? abb_pertenece_rec(arbol->raiz, clave, arbol->cmp) : false;
}

static size_t abb_cantidad_rec(abb_nodo_t *nodo) {
	return (nodo) ? abb_cantidad_rec(nodo->izq) + abb_cantidad_rec(nodo->der) +1 : 0;
}

size_t abb_cantidad(const abb_t *arbol) {
	return (arbol->raiz) ? abb_cantidad_rec(arbol->raiz) : 0;
}

static void abb_destruir_rec(abb_nodo_t ** nodo, abb_destruir_dato_t destruir_dato) {
    if(!*nodo) return;
    abb_destruir_rec(&((*nodo)->izq), destruir_dato);
    abb_destruir_rec(&((*nodo)->der), destruir_dato);
    if (destruir_dato) destruir_dato((*nodo)->dato);
    free((*nodo)->clave);
    free((*nodo));
}

void abb_destruir(abb_t *arbol) {
    abb_destruir_rec(&arbol->raiz, arbol->destruir_dato);
    free(arbol);
}



// Iterador interno
bool abb_in_order_(abb_nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra) {
    if (nodo == NULL) return true;
    if(!abb_in_order_(nodo->izq, visitar, extra)) return false;
    if(!visitar(nodo->clave, nodo->dato, extra)) return false;
    if(!abb_in_order_(nodo->der, visitar, extra)) return false;
    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    if (visitar == NULL) return;
    abb_in_order_(arbol->raiz, visitar, extra);
}

// Iterador externo

/*
Recorrido inorder (pila)
- Crear
    Apila raíz y todo los hijos izquierdo
- Avanzar
    ¿Está al final?
    Desapilo
    Apilo hijo derecho del desapilado (si existe) y todos sus hijos izquierdos
- Ver actual
    Veo el tope de la pila
- ¿Está al final?
    Verifico si la pila está vacía
- Destruir
    Destruyo la pila
*/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;
    iter->pila = pila_crear();
    if (!iter->pila) {
        free(iter);
        return NULL;
    }
    if(!arbol->raiz) return iter;
    pila_apilar(iter->pila, arbol->raiz);
    if (!arbol->raiz) return iter;
    abb_nodo_t *nodo = arbol->raiz->izq;
    while (nodo != NULL){
        pila_apilar(iter->pila, nodo);
        nodo = nodo->izq;
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) return false;
    abb_nodo_t *nodo = pila_desapilar(iter->pila);
    if(!nodo) return false;
    if (nodo->der) {
        pila_apilar(iter->pila, nodo->der);
        nodo = nodo->der;
        while (nodo->izq) {
            pila_apilar(iter->pila, nodo->izq);
            nodo = nodo->izq;
        }
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) return NULL;
    abb_nodo_t *nodo = pila_ver_tope(iter->pila);
    return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}
