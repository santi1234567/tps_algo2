#include "heap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_INICIAL 21

struct heap {
    void **elementos;
    size_t cantidad;
    size_t capacidad;
	cmp_func_t cmp;
};

static void swap(void **x, void **y) {
	void *aux = *x;
	*x = *y;
	*y = aux;
}

static void heap_upheap(void *elementos[], size_t hijo, cmp_func_t cmp);
static void heap_downheap(void *elementos[], size_t tam, size_t padre, cmp_func_t cmp);
static bool heap_redimensionar(heap_t *heap, size_t nueva_capacidad);


static size_t max(void *elementos[], size_t pos_x, size_t pos_y, cmp_func_t cmp) {
 	return (cmp(elementos[pos_x], elementos[pos_y]) < 0) ? pos_y : pos_x;
}

static void heapify(void *elementos[], size_t cant, cmp_func_t cmp) {
	for (size_t i = 0; i < cant; i++) {
		heap_downheap(elementos, cant, cant-i-1, cmp);
	}
}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
	heapify(elementos, cant, cmp);
	for (size_t i = 0; i < cant; i++) {
		swap(elementos[0], elementos[cant-i-1]);
		heap_downheap(elementos, cant-i, 0, cmp);
	}
}

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t *heap_crear(cmp_func_t cmp) {
	heap_t *heap = malloc(sizeof(heap_t));
	if (!heap) {
		return NULL;
	}

	void **elementos = malloc(sizeof(void*)*TAM_INICIAL);
	if (!elementos) {
		free(heap);
		return NULL;
	}
	heap->elementos = elementos;
	heap->cantidad = 0;
	heap->capacidad = TAM_INICIAL;
	heap->cmp = cmp;
	return heap;
}

/*
 * Constructor alternativo del heap. Además de la función de comparación,
 * recibe un arreglo de valores con que inicializar el heap. Complejidad
 * O(n).
 *
 * Excepto por la complejidad, es equivalente a crear un heap vacío y encolar
 * los valores de uno en uno
*/

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t *heap = heap_crear(cmp);
	if (!heap) {
		return NULL;
	}
	heapify(arreglo, n, cmp);
	for (size_t i = 0; i < n; i++) {
		if (!heap_encolar(heap, arreglo[i])) {
			heap_destruir(heap, NULL);
			return NULL;
		}
	}
	return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
	if (destruir_elemento) {
		for (size_t i = 0; i < heap->cantidad; i++) {
			destruir_elemento(heap->elementos[i]);
		}
	}
	free(heap->elementos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap) {
	return !(heap->cantidad);
}

static void heap_upheap(void *elementos[], size_t hijo, cmp_func_t cmp){
	if (hijo != 0) { // caso base de primer elemento
		size_t padre = (hijo-1)/2;
		if (cmp(elementos[hijo], elementos[padre]) > 0) {
			swap(&elementos[padre], &elementos[hijo]);
			heap_upheap(elementos, padre, cmp);
		}
	}
}

bool heap_encolar(heap_t *heap, void *elem) {
	if (heap->cantidad == heap->capacidad) {
		if(!heap_redimensionar(heap, heap->capacidad*2)) {
			return false;
		}
	}
	heap->elementos[heap->cantidad] = elem;
	heap->cantidad++;

	heap_upheap(heap->elementos, heap->cantidad-1, heap->cmp);
	return true;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void *heap_ver_max(const heap_t *heap) {
	return (heap->cantidad>0) ? heap->elementos[0] : NULL;
}



static void heap_downheap(void *elementos[], size_t tam, size_t padre, cmp_func_t cmp){
	printf("Pos Padre es %lu\n",padre );
	if (2*padre+1 < tam) { //caso base para las hojas
		size_t hijo_izq = 2*padre+1;
		size_t hijo_der = 2*padre+2;
		printf("Pos hijo izq es %lu, Pos hijo der es %lu, cantidad es %lu\n",hijo_izq, hijo_der, tam );
		size_t hijo_max;
		if (hijo_der == tam) { // si no tiene hijo derecho
			hijo_max = hijo_izq;
		} else {
			hijo_max = max(elementos, hijo_izq, hijo_der, cmp);
		}
		printf("Padre es %i, Hijo max es %i\n", *(int *)elementos[padre],*(int *)elementos[hijo_max] );
		if (cmp(elementos[padre], elementos[hijo_max]) < 0) { 
			swap(&elementos[padre], &elementos[hijo_max]);
		}
		printf("Nuevo Padre es %i, Nuevo Hijo max es %i\n", *(int *)elementos[padre],*(int *)elementos[hijo_max] );

		heap_downheap(elementos, tam, hijo_max, cmp);
	}
}


/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void *heap_desencolar(heap_t *heap) {
	if (heap_esta_vacio(heap)) {
		return NULL;
	}
	for (size_t i = 0; i < heap->cantidad; i++) {
		printf("Elemento %lu es %i\n",i, *(int*)heap->elementos[i] );
	}

	swap(&heap->elementos[heap->cantidad-1], &heap->elementos[0]);
	void *aux = heap->elementos[heap->cantidad-1];
	heap->elementos[heap->cantidad-1] = NULL;
	heap->cantidad--;
	heap_downheap(heap->elementos, heap->cantidad, 0, heap->cmp);
	if (heap->cantidad == heap->capacidad/4) {
		if(!heap_redimensionar(heap, heap->capacidad/2)) {
			return NULL;
		}
	}
	for (size_t i = 0; i < heap->cantidad; i++) {
		printf("Nuevo Elemento %lu es %i\n",i, *(int*)heap->elementos[i] );
	}

	return aux;
}


static bool heap_redimensionar(heap_t *heap, size_t nueva_capacidad) {
	void **aux = realloc(heap->elementos, nueva_capacidad*sizeof(void*));
	if (!aux) {
		return false;
	}
	heap->elementos = aux;
	heap->capacidad = nueva_capacidad;
	return true;
}
