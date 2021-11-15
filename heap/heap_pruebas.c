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

static int intcmp(const void *x, const void *y) {
	return *(int*)x-*(int *)y;
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


static void prueba_heap_volumen(size_t largo, bool debug)
{
	puts("\nINICIO PRUEBAS VOLUMEN");
    heap_t* heap = heap_crear(strcmp, free);
    const size_t largo_clave = 20;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    unsigned* valores[largo];

    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", rand());
        *valores[i] = i;
        ok = heap_guardar(heap, claves[i], valores[i]);
        if (!ok) break;
    }

    if (debug) print_test("Prueba heap almacenar muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

    for (size_t i = 0; i < largo; i++) {
        ok = heap_pertenece(heap, claves[i]);
        if (!ok) break;
        ok = heap_obtener(heap, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba heap pertenece y obtener muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

    for (size_t i = 0; i < largo; i++) {
        ok = heap_borrar(heap, claves[i]) == valores[i];
        if (!ok) break;
    }

    if (debug) print_test("Prueba heap borrar muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap);
    heap = heap_crear(strcmp, free);

    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = heap_guardar(heap, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);

    heap_destruir(heap);

}
/*

static ssize_t buscar(const char* clave, char* claves[], size_t largo)
{
    for (size_t i = 0; i < largo; i++) {
        if (strcmp(clave, claves[i]) == 0) return (ssize_t) i;
    }
    return -1;
}

static void prueba_heap_iterar()
{
	puts("\nINICIO PRUEBAS ITERAR");
    heap_t* heap = heap_crear(strcmp, NULL);

    char *claves[] = {"perro", "gato", "vaca"};
    char *valores[] = {"guau", "miau", "mu"};

    print_test("Prueba heap insertar clave1", heap_guardar(heap, claves[0], valores[0]));
    print_test("Prueba heap insertar clave2", heap_guardar(heap, claves[1], valores[1]));
    print_test("Prueba heap insertar clave3", heap_guardar(heap, claves[2], valores[2]));

    // Prueba de iteración sobre las claves almacenadas.
    heap_iter_t* iter = heap_iter_in_crear(heap);
    const char *clave;
    ssize_t indice;

    print_test("Prueba heap iterador esta al final, es false", !heap_iter_in_al_final(iter));

    clave = heap_iter_in_ver_actual(iter);
	indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
	print_test("Prueba heap iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba heap iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba heap iterador avanzar es true", heap_iter_in_avanzar(iter));
    print_test("Prueba heap iterador esta al final, es false", !heap_iter_in_al_final(iter));
    clave = heap_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba heap iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba heap iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    print_test("Prueba heap iterador avanzar es true", heap_iter_in_avanzar(iter));
    print_test("Prueba heap iterador esta al final, es false", !heap_iter_in_al_final(iter));

    clave = heap_iter_in_ver_actual(iter);
    indice = buscar(clave, claves, sizeof(claves) / sizeof(char *));
    print_test("Prueba heap iterador ver actual, es una clave valida", indice != -1);
    print_test("Prueba heap iterador ver actual, no es el mismo puntero", clave != claves[indice]);
    heap_iter_in_avanzar(iter);
    print_test("Prueba heap iterador esta al final, es true", heap_iter_in_al_final(iter));

    print_test("Prueba heap iterador ver actual, es NULL", !heap_iter_in_ver_actual(iter));
    print_test("Prueba heap iterador avanzar es false", !heap_iter_in_avanzar(iter));
    print_test("Prueba heap iterador esta al final, es true", heap_iter_in_al_final(iter));

    heap_iter_in_destruir(iter);
    heap_destruir(heap);
}

static void prueba_heap_iterar_volumen(size_t largo)
{
	puts("\nINICIO PRUEBAS ITERAR VOLUMEN");

    heap_t* heap = heap_crear(strcmp, NULL);

    const size_t largo_clave = 20;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];


    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", rand());
        valores[i] = i;
        ok = heap_guardar(heap, claves[i], &valores[i]);
        if (!ok) break;
    }

    // Prueba de iteración sobre las claves almacenadas.
    heap_iter_t* iter = heap_iter_in_crear(heap);
    print_test("Prueba heap iterador esta al final, es false", !heap_iter_in_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( heap_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = heap_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = heap_obtener(heap, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        heap_iter_in_avanzar(iter);
    }
    print_test("Prueba heap iteración en volumen", ok);
    print_test("Prueba heap iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba heap iterador esta al final, es true", heap_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba heap iteración en volumen, se cambiaron todo los elementos", ok);

    free(claves);
    heap_iter_in_destruir(iter);
    heap_destruir(heap);
}
*/
/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

void pruebas_heap_estudiante()
{
    /* Ejecuta todas las pruebas unitarias. */
	prueba_crear_heap_vacio();
    prueba_heap_encolar_desencolar();
	/*
    prueba_heap_reemplazar();
    prueba_heap_reemplazar_con_destruir();
    prueba_heap_borrar();
    prueba_heap_clave_vacia();
    prueba_heap_valor_null();
    prueba_heap_volumen(1000, true);
	*/
}
