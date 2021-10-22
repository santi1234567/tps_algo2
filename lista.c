#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



typedef struct nodo {
    void *dato;
    struct nodo *prox;
} nodo_t;

struct lista {
    nodo_t *primero;
    nodo_t *ultimo;
    size_t largo;
};

struct lista_iter {
	lista_t* lista;
	struct nodo* act;
	struct nodo* ant;
};


static nodo_t* crear_nodo(void *dato) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) return NULL;
    nodo->dato = dato;
    nodo->prox = NULL;
    return nodo;
}

static void destruir_nodo(nodo_t *nodo){
    free(nodo);
}

lista_t *lista_crear() {
    lista_t *lista = malloc(sizeof(lista_t));
    if (lista == NULL)
        return NULL;
    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return !lista->largo;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nodo = crear_nodo(dato);
    if (nodo == NULL)
        return false;
    nodo->prox = lista->primero;
    lista->primero = nodo;
    lista->largo ++;
    if (lista->ultimo == NULL)
        lista->ultimo = nodo;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t *nodo = crear_nodo(dato);
    if (nodo == NULL)
        return false;
    if (lista->ultimo == NULL)
        lista->primero = nodo;
    else
        lista->ultimo->prox = nodo;
    lista->ultimo = nodo;
    lista->largo ++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista->primero == NULL)
        return NULL;
    nodo_t *aux = lista->primero;
    void *dato = aux->dato;
    lista->primero = lista->primero->prox;
    destruir_nodo(aux);
    lista->largo --;
    if(lista->primero == NULL)
        lista->ultimo = NULL;
    return dato;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista->primero == NULL) return NULL;
    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t *lista) {
    if (lista->ultimo == NULL) return NULL;
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while (lista->primero != NULL) {
        nodo_t *aux = lista->primero;
        if (destruir_dato)
            destruir_dato(aux->dato);
        lista->primero = aux->prox;
        free(aux);
    }
    free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
	if(lista_esta_vacia(lista) || visitar == NULL)
		return;
	nodo_t *nodo = lista->primero;
	while (visitar(nodo->dato, extra)) {
		nodo = nodo->prox;
		if(nodo == NULL)
			break;
	}
}


// Primitivas Iterador

lista_iter_t *lista_iter_crear(lista_t *lista) {
	if (lista == NULL)
		return NULL;
	lista_iter_t *iter = malloc(sizeof(lista_iter_t));
    if (iter == NULL)
        return NULL;
    iter->lista = lista;
    iter->act = lista->primero;
    iter->ant = NULL;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter))
        return false;
    nodo_t *aux = iter->act;
    iter->act = iter->act->prox;
    iter->ant = aux;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    return lista_iter_al_final(iter) ? NULL : iter->act->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->act == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    if(lista_iter_al_final(iter)) {
        if(!lista_insertar_ultimo(iter->lista, dato))
			return false;
		else {
			iter->act = iter->lista->ultimo;
			return true;
		}
	}
	if(iter->ant == NULL) {
		if(!lista_insertar_primero(iter->lista, dato))
			return false;
		else {
			iter->act = iter->lista->primero;
			return true;
		}
	}
	nodo_t *nodo = crear_nodo(dato);
	if (nodo == NULL)
		return false;
	nodo->prox = iter->act;
	iter->ant->prox = nodo;
	iter->act = nodo;
	iter->lista->largo++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
	if(lista_iter_al_final(iter))
		return NULL;
	void *dato;
	if(iter->ant == NULL) {
		dato = lista_borrar_primero(iter->lista);
		if(dato == NULL)
		return NULL;
		else {
			iter->act = iter->lista->primero;
			return dato;
		}
	}
	if(iter->act == iter->lista->ultimo) {
	    nodo_t *aux = iter->lista->ultimo;
		dato = aux->dato;
	    iter->lista->ultimo = iter->ant;
	    if(iter->ant != NULL)
	        iter->lista->ultimo->prox = NULL;
	    iter->act = NULL;
		iter->lista->largo --;
	    destruir_nodo(aux);
		return dato;
	}
	dato = iter->act->dato;
	nodo_t *aux = iter->act->prox;
	destruir_nodo(iter->act);
	iter->act = aux;
	iter->ant->prox = aux;
	iter->lista->largo --;
	return dato;
}
