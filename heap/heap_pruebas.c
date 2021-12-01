/*
 * prueba_heap.c
 * Pruebas para el tipo de dato abstracto Tabla de heap
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 */

#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include <time.h>

#define CANT_VOLUMEN 10000

static int intcmp(const void *x, const void *y) {
	return *(int*)x-*(int *)y;
}
static int intcmp_inv(const void *x, const void *y) { //para que se ordene el arreglo de mayor a menor con heap_sort
	return -1*intcmp(x, y);
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

static void prueba_crear_heap_vacio()
{
	printf("\nINICIO PRUEBAS HEAP VACIO\n");
    heap_t* heap = heap_crear(intcmp);
    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap ver max es NULL", !heap_ver_max(heap));
    print_test("Prueba heap desencolar es NULL", !heap_desencolar(heap));
    heap_destruir(heap, NULL);
}


static void prueba_heap_encolar_desencolar()
{
    heap_t* heap = heap_crear(intcmp);
    puts("\nINICIO PRUEBAS INSERTAR");

	int valores[6] = {5, 9, 10, 12, 2, -15};

    print_test("Prueba heap encolar 5", heap_encolar(heap, &valores[0]));
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap ver max es 5", *(int *)heap_ver_max(heap) == 5);

	print_test("Prueba heap encolar 9", heap_encolar(heap, &valores[1]));
	print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap ver max es 9", *(int *)heap_ver_max(heap) == 9);

	print_test("Prueba heap encolar 10", heap_encolar(heap, &valores[2]));
	print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
    print_test("Prueba heap ver max es 10", *(int *)heap_ver_max(heap) == 10);

	print_test("Prueba heap encolar 12", heap_encolar(heap, &valores[3]));
	print_test("Prueba heap la cantidad de elementos es 4", heap_cantidad(heap) == 4);
    print_test("Prueba heap ver max es 12", *(int *)heap_ver_max(heap) == 12);

	print_test("Prueba heap encolar 2", heap_encolar(heap, &valores[4]));
	print_test("Prueba heap la cantidad de elementos es 5", heap_cantidad(heap) == 5);
    print_test("Prueba heap ver max es 12", *(int *)heap_ver_max(heap) == 12);

	print_test("Prueba heap encolar -15", heap_encolar(heap, &valores[5]));
	print_test("Prueba heap la cantidad de elementos es 6", heap_cantidad(heap) == 6);
    print_test("Prueba heap ver max es 12", *(int *)heap_ver_max(heap) == 12);

	print_test("Prueba heap desencolar es 12", *(int *)heap_desencolar(heap) == 12);
	print_test("Prueba heap desencolar es 10", *(int *)heap_desencolar(heap) == 10);
	print_test("Prueba heap desencolar es 9", *(int *)heap_desencolar(heap) == 9);
	print_test("Prueba heap desencolar es 5", *(int *)heap_desencolar(heap) == 5);
	print_test("Prueba heap desencolar es 2", *(int *)heap_desencolar(heap) == 2);
	print_test("Prueba heap desencolar es -15", *(int *)heap_desencolar(heap) == -15);
    heap_destruir(heap, NULL);
}


static void prueba_heap_volumen(size_t largo)
{
	puts("\nINICIO PRUEBAS VOLUMEN");
    heap_t* heap = heap_crear(intcmp);

    int* valores[largo];
	int* valores2[largo];
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        *valores[i] = rand()/100000;
		valores2[i] = malloc(sizeof(int));
		*valores2[i]=*valores[i];
		ok = heap_encolar(heap, valores[i]);
        if (!ok) break;
    }
	print_test("Prueba heap almacenar muchos elementos", ok);
	print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

	heap_sort((void**)valores2, largo, intcmp_inv);
	for (size_t i = 0; i < largo-1; i++) {
		ok = *valores2[i] >= *valores2[i+1];
		if (!ok) break;
	}
	print_test("Prueba heap_sort ordena de forma correcta", ok);

    for (size_t i = 0; i < largo; i++) {
		int valor_desencolado = *(int *)heap_desencolar(heap);
        ok = valor_desencolado == *valores2[i];
        if (!ok) break;
		ok = heap_cantidad(heap) == largo-i-1;
		if (!ok) break;
    }

    print_test("Prueba heap desencolar", ok);
    print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == 0);
	for (size_t i = 0; i < largo; i++) {
		free(valores2[i]);
		free(valores[i]);
	}
    heap_destruir(heap, NULL);

	heap = heap_crear(intcmp);

    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        *valores[i] = rand()/100000;
		ok = heap_encolar(heap, valores[i]);
        if (!ok) break;
    }

    print_test("Prueba heap almacenar muchos elementos", ok);
	print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);
    heap_destruir(heap, free);
	print_test("Prueba destruir con muchos elementos usando free", true);

}

void prueba_crear_heap_arr() {
	puts("\nINICIO PRUEBAS CREAR HEAP CON ARREGLO");
	int valores[6] = {5, 9, 10, 12, 2, -15};

	int *arreglo[6];

	for (size_t i = 0; i < 6; i++) {
		arreglo[i] = malloc(sizeof(int));
		*arreglo[i] = valores[i];
	}
	heap_t* heap = heap_crear_arr((void **)arreglo, 6, intcmp);
	print_test("Prueba heap crear heap con arreglo", heap);
	print_test("Prueba heap la cantidad de elementos es 6", heap_cantidad(heap) == 6);
	print_test("Prueba heap ver max es 12", *(int *)heap_ver_max(heap) == 12);
	print_test("Prueba heap desencolar es 12", *(int *)heap_desencolar(heap) == 12);
	print_test("Prueba heap desencolar es 10", *(int *)heap_desencolar(heap) == 10);
	print_test("Prueba heap desencolar es 9", *(int *)heap_desencolar(heap) == 9);
	print_test("Prueba heap desencolar es 5", *(int *)heap_desencolar(heap) == 5);
	print_test("Prueba heap desencolar es 2", *(int *)heap_desencolar(heap) == 2);
	print_test("Prueba heap desencolar es -15", *(int *)heap_desencolar(heap) == -15);
	heap_destruir(heap, NULL);

	heap = heap_crear_arr((void **)arreglo, 6, intcmp);
	heap_destruir(heap, free);
	print_test("Prueba destruir con muchos elementos usando free", true);

}

void prueba_crear_heap_arr_volumen(size_t largo) {
	puts("\nINICIO PRUEBAS CREAR HEAP CON ARREGLO VOLUMEN");

	int* valores[largo];
	int* valores2[largo];
	bool ok = true;
	for (unsigned i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		*valores[i] = rand()/100000;
		valores2[i] = malloc(sizeof(int));
		*valores2[i]=*valores[i];
	}
	heap_t* heap = heap_crear_arr((void**)valores, largo, intcmp);
	print_test("Prueba heap crear heap con arreglo", heap);
	print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

	for (unsigned i = 0; i < largo; i++) {
		ok = *valores[i] == *valores2[i];
		if (!ok) break;
	}
	print_test("Prueba heap crear heap con arreglo, arreglo original mantiene orden", ok);

	heap_sort((void**)valores2, largo, intcmp_inv);
	for (size_t i = 0; i < largo; i++) {
		int valor_desencolado = *(int *)heap_desencolar(heap);
		ok = valor_desencolado == *valores2[i];
		if (!ok) break;
		ok = heap_cantidad(heap) == largo-i-1;
		if (!ok) break;
	}
	print_test("Prueba heap desencolar", ok);
	print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == 0);
	for (size_t i = 0; i < largo; i++) {
		free(valores2[i]);
		free(valores[i]);
	}
	heap_destruir(heap, NULL);


	for (unsigned i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		*valores[i] = rand()/100000;
	}
	heap = heap_crear_arr((void**)valores, largo, intcmp);

	print_test("Prueba heap crear heap con arreglo", heap);
	print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);
	heap_destruir(heap, free);
	print_test("Prueba destruir con muchos elementos usando free", true);
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_heap_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */
	prueba_crear_heap_vacio();
	prueba_crear_heap_arr();
    prueba_heap_encolar_desencolar();
    prueba_heap_volumen(CANT_VOLUMEN);
	prueba_crear_heap_arr_volumen(CANT_VOLUMEN);
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
