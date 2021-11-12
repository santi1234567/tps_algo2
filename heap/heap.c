#include "heap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_INICIAL 21

struct heap {
    void *elementos[];
    size_t cantidad;
    size_t capacidad;
	cmp_func_t *cmp;
};

static void swap(void *x, void *y) {
	void *aux = x;
	x = y;
	y = aux;
}

static size_t min(const void *elementos, size_t pos_x, size_t pos_y, cmp_func_t cmp) {
 	return (cmp(elementos[pos_x], elementos[pos_x]) > 0) ? pos_y : pos_x;
}

/* Función de heapsort genérica. Esta función ordena mediante heap_sort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Nótese que esta función NO es formalmente parte del TAD Heap.
 */
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp);

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t *heap_crear(cmp_func_t cmp) {
	heap_t *heap = malloc(sizeof(heap_t));
	if (!heal) {
		return NULL;
	}

	elementos = malloc(sizeof(void*)*TAM_INICIAL);
	if (!elementos) {
		free(heap);
		return NULL;
	}
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
	if (!heal) {
		return NULL;
	}
	heap_sort(arreglo, n, cmp);
	for (size_t i = 0; i < n; i++) {
		if (!heap_encolar(heap, arreglo[i])) {
			heap_destruir(heap);
			return NULL;
		}
	}
	return heap;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
	if (destruir_elemento) {
		for (size_t i = 0; i < heap->cantidad; i++) {
			destruir_elemento(heap->elementos[i]);
		}
	}
	free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap) {
	return heap->cantidad;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap) {
	return !(heap->cantidad);
}

void heap_upheap(void *elementos[], size_t hijo, cmp_func_t cmp){
	if (hijo == 0) { // caso base de primer elemento
		return;
	}
	size_t padre = (hijo-1)/2;
	if (cmp(elementos[hijo], elementos[padre]) > 0) {
		swap(elementos[padre], elementos[hijo]);
		return heap_upheap(elementos, padre, cmp);
	}
}

/* Agrega un elemento al heap.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
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
	return heap->elementos[0];
}



void heap_downheap(void *elementos[], size_t tam, size_t padre, cmp_func_t cmp){
	if (2*padre+1 > tam) { //caso base para las hojas
		return;
	}
	size_t hijo_izq = 2*padre+1;
	size_t hijo_der = 2*padre+2;
	size_t hijo_min = min(elementos, hijo_izq, hijo_der, cmp);
	swap(elementos[padre], elementos[hijo_min]);
	return heap_downheap(elementos, tam, hijo_min, cmp);
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
	swap(heap->elementos[heap->cantidad-1], heap->elementos[0]);
	void *aux = heap->elementos[heap->cantidad-1];
	heap->elementos[heap->cantidad-1] = NULL;
	heap->cantidad--;
	heap_downheap(heap->elementos, heap->cantidad, 0, heap->cmp);
	if (heap->cantidad == heap->capacidad/4) {
		if(!heap_redimensionar(heap, heap->capacidad/2)) {
			return NULL;
		}
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
