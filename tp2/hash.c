#include "lista.h"
#include "hash.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_INICIAL 20

typedef struct nodo {
    char *clave;
    void *dato;
} nodo_hash_t;

struct hash {
    lista_t **listas;
    size_t cantidad;
    size_t capacidad;
    hash_destruir_dato_t destruir_dato;
};


struct hash_iter {
	size_t pos;
	const hash_t* hash;
	lista_iter_t* lista_iter;
};


/*
http://www.cse.yorku.ca/~oz/hash.html
sdbm
this algorithm was created for sdbm (a public-domain reimplementation of ndbm) database library. it was found to do well in scrambling bits, causing better distribution of the keys and fewer splits. it also happens to be a good general hashing function with good distribution. the actual function is hash(i) = hash(i - 1) * 65599 + str[i]; what is included below is the faster version used in gawk. [there is even a faster, duff-device version] the magic constant 65599 was picked out of thin air while experimenting with different constants, and turns out to be a prime. this is one of the algorithms used in berkeley db (see sleepycat) and elsewhere.
*/

static size_t funcion_hash(const char *str, size_t capacidad){
    unsigned long hash = 0;
    int c;
    while ((c = *str++))
        hash = (unsigned long)c + (hash << 6) + (hash << 16) - hash;

    return hash%capacidad;
}

static nodo_hash_t *hallar_clave(const char *clave, lista_iter_t *iter) {
    while (!lista_iter_al_final(iter)) {
        nodo_hash_t *nodo = lista_iter_ver_actual(iter);
        if (!strcmp(nodo->clave, clave))
            return nodo;
        lista_iter_avanzar(iter);
    }
    return NULL;
}

static bool hash_redimensionar(hash_t *hash, size_t nuevo_tam) {
    lista_t **listas2 = malloc(sizeof(lista_t*)*nuevo_tam);
    if (!listas2) return false;

    for (size_t i = 0; i < nuevo_tam; i++) {
        listas2[i] = lista_crear();
        if (!listas2[i]){
            while (i--)
                free(listas2[i]);
            free(listas2);
            return false;
        }
    }

    for (size_t i = 0; i < hash->capacidad; i++) {
        while (!lista_esta_vacia(hash->listas[i])) {
            nodo_hash_t *nodo = lista_borrar_primero(hash->listas[i]);
            size_t indice = funcion_hash(nodo->clave, nuevo_tam);
            lista_insertar_ultimo(listas2[indice], nodo);
        }
        free(hash->listas[i]);
    }

    free(hash->listas);
    hash->listas = listas2;
    hash->capacidad = nuevo_tam;
    return true;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    hash->listas = malloc(sizeof(lista_t*)*TAM_INICIAL);
    if (!hash->listas) {
        free(hash);
        return NULL;
    }

    for (size_t i = 0; i < TAM_INICIAL; i++) {
        hash->listas[i] = lista_crear();
        if (!hash->listas[i]){
            while (i--)
                free(hash->listas[i]);
            free(hash->listas);
            free(hash);
            return NULL;
        }
    }

    hash->cantidad = 0;
    hash->capacidad = TAM_INICIAL;
    hash->destruir_dato = destruir_dato;
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if (hash->cantidad/hash->capacidad > 3)
        if (!hash_redimensionar(hash, hash->capacidad*2)) return false;

    size_t indice = funcion_hash(clave, hash->capacidad);
    nodo_hash_t *aux;
    lista_iter_t *iter = lista_iter_crear(hash->listas[indice]);
    if ((aux = hallar_clave(clave, iter)) != NULL) {
        if (hash->destruir_dato)
            hash->destruir_dato(aux->dato);
        aux->dato = dato;
        lista_iter_destruir(iter);
        return true;
    }
    lista_iter_destruir(iter);
    nodo_hash_t *nodo = malloc(sizeof(nodo_hash_t));
    if (!nodo) return false;
	nodo->clave = malloc(sizeof(char)*(strlen(clave)+1));
	if (!nodo->clave) return false;
	strcpy(nodo->clave,clave);
	//nodo->clave = strdup(clave);
    nodo->dato = dato;
    if (!lista_insertar_ultimo(hash->listas[indice], nodo)) {
        free(nodo);
        return false;
    }
    hash->cantidad++;
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    size_t indice = funcion_hash(clave, hash->capacidad);
    nodo_hash_t *nodo;
    lista_iter_t *iter = lista_iter_crear(hash->listas[indice]);
    if (!(nodo = hallar_clave(clave, iter))) {
        lista_iter_destruir(iter);
        return NULL;
    }
    void *dato = nodo->dato;
    free(nodo->clave);
    free(nodo);
    lista_iter_borrar(iter);
    lista_iter_destruir(iter);
    hash->cantidad--;
    if (hash->cantidad/hash->capacidad < 3 && hash->cantidad > TAM_INICIAL)
        hash_redimensionar(hash, hash->capacidad/2);
    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    lista_iter_t *iter = lista_iter_crear(hash->listas[funcion_hash(clave, hash->capacidad)]);
    if (!iter) return NULL;
    nodo_hash_t *nodo = hallar_clave(clave, iter);
    lista_iter_destruir(iter);

    return nodo? nodo->dato: NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    lista_iter_t *iter = lista_iter_crear(hash->listas[funcion_hash(clave, hash->capacidad)]);
    if (!iter) return NULL;

    void *dato = hallar_clave(clave, iter);
    lista_iter_destruir(iter);
    return dato? true: false;
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
    for (size_t i = 0; i < hash->capacidad; i++) {
        while (!lista_esta_vacia(hash->listas[i])){
            nodo_hash_t *nodo = lista_borrar_primero(hash->listas[i]);
            if (hash->destruir_dato)
                hash->destruir_dato(nodo->dato);
            free(nodo->clave);
            free(nodo);
        }
        lista_destruir(hash->listas[i], free);
    }
    free(hash->listas);
    free(hash);
}


/* Iterador del hash */

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	if (hash == NULL)
		return NULL;
	hash_iter_t *iter = malloc(sizeof(hash_iter_t));
	if (iter == NULL)
		return NULL;
	iter->hash = hash;
	if (hash_cantidad(iter->hash) == 0) {
		iter->pos = iter->hash->capacidad;
		iter->lista_iter = NULL;
		return iter;
	}
	iter->pos = 0;
	while (lista_esta_vacia(iter->hash->listas[iter->pos]) && !hash_iter_al_final(iter))
		iter->pos++;

	if (hash_iter_al_final(iter)) {
		iter->lista_iter = NULL;
		return iter;
	}

	iter->lista_iter = lista_iter_crear(hash->listas[iter->pos]);
	if (iter->lista_iter == NULL) {
		free(iter);
		return NULL;
	}
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if (hash_iter_al_final(iter))
		return false;
	if (!lista_iter_avanzar(iter->lista_iter) || lista_iter_al_final(iter->lista_iter)) {
		lista_iter_destruir(iter->lista_iter);
		do
			iter->pos ++;
		while (!hash_iter_al_final(iter) && lista_esta_vacia(iter->hash->listas[iter->pos]));
		if (!hash_iter_al_final(iter))
			iter->lista_iter = lista_iter_crear(iter->hash->listas[iter->pos]);
		else
			iter->lista_iter = NULL;
		return true;
	}
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	return (!iter->lista_iter) ? NULL : *(char**)lista_iter_ver_actual(iter->lista_iter);
}


bool hash_iter_al_final(const hash_iter_t *iter) {
	return (iter->pos == iter->hash->capacidad);
}


void hash_iter_destruir(hash_iter_t *iter) {
	lista_iter_destruir(iter->lista_iter);
	free(iter);
}
