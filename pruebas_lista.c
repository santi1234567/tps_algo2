#include "lista.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>

#define CANT_PRUEBA_VOLUMEN 400

void prueba_iter_volumen() {
	printf("%s\n","INICIO PRUEBAS ITER VOLUMEN...");
	lista_t *lista = lista_crear();
	lista_iter_t *iter = lista_iter_crear(lista);
	bool estado=true;
    int i;
    int datos1[CANT_PRUEBA_VOLUMEN];
	for (i = 0; i < CANT_PRUEBA_VOLUMEN; i++) {
        datos1[i] = i;
        if(!lista_iter_insertar(iter, &datos1[i])) {
			estado=false;
			break;
		}
    }
	print_test("\titer insertar muchos elementos",estado);
	estado = true;
	for (i = 0; i < CANT_PRUEBA_VOLUMEN; i++) {
        if(*(int*)lista_iter_ver_actual(iter) != datos1[CANT_PRUEBA_VOLUMEN-i-1] || *(int*)lista_iter_borrar(iter) != datos1[CANT_PRUEBA_VOLUMEN-i-1]) {
			estado = false;
			break;
		}
    }
	print_test("\tInvariante iter borrar desde el primero", estado);
	estado = true;
	for (i = 0; i < CANT_PRUEBA_VOLUMEN; i++) {
        if(!lista_iter_insertar(iter, &datos1[i])) {
			estado = false;
			break;
		}
    }
	print_test("\titer insertar muchos elementos", estado);
	estado = true;
	for (i = 0; i < CANT_PRUEBA_VOLUMEN/2; i++) {
        if(!lista_iter_avanzar(iter)) {
			estado = false;
			break;
		}
    }
	print_test("\titer avanzar a la mitad", estado);
	estado = true;
	for (i = CANT_PRUEBA_VOLUMEN/2; i < CANT_PRUEBA_VOLUMEN; i++) {
		if(*(int*)lista_iter_ver_actual(iter) != datos1[CANT_PRUEBA_VOLUMEN-i-1] || *(int*)lista_iter_borrar(iter) != datos1[CANT_PRUEBA_VOLUMEN-i-1]) {
			estado = false;
			break;
		}
	}

	print_test("\tInvariante iter borrar desde la mitad", estado);
	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void prueba_iter_crear_destruir() {
	printf("%s\n","INICIO PRUEBAS ITER LISTA NULL...");
	lista_iter_t *iter = lista_iter_crear(NULL);
	print_test("\titer crear sin lista == NULL", iter == NULL);

	printf("%s\n","INICIO PRUEBAS ITER LISTA VACIA...");
	lista_t *lista = lista_crear();
    iter = lista_iter_crear(lista);
    print_test("\titer crear != NULL", iter != NULL);
	print_test("\titer al final", lista_iter_al_final(iter));
	print_test("\titer avanzar", !lista_iter_avanzar(iter));
	print_test("\titer ver actual", lista_iter_ver_actual(iter) == NULL);
    lista_iter_destruir(iter);
    print_test("\titer destruir", true);

	printf("%s\n","INICIO PRUEBAS ITER LISTA CON ELEMENTOS 3->2->1->0...");
	int valores[4];
	for(size_t i = 0; i < 4; i++) {
		valores[i] = (int)i;
		lista_insertar_primero(lista, &valores[i]);
	}
	iter = lista_iter_crear(lista);
	print_test("\titer crear != NULL", iter != NULL);
	print_test("\titer no esta al final", !lista_iter_al_final(iter));
	print_test("\titer ver actual = 3", *(int*)lista_iter_ver_actual(iter) == 3);
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer no esta al final", !lista_iter_al_final(iter));

	print_test("\titer ver actual = 2", *(int*)lista_iter_ver_actual(iter) == 2);
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer ver actual = 1", *(int*)lista_iter_ver_actual(iter) == 1);
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer ver actual = 0", *(int*)lista_iter_ver_actual(iter) == 0);
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer al final", lista_iter_al_final(iter));
	print_test("\titer avanzar", !lista_iter_avanzar(iter));
	print_test("\titer ver actual", lista_iter_ver_actual(iter) == NULL);
	lista_destruir(lista, NULL);

	lista_iter_destruir(iter);
	print_test("\titer destruir", true);
}

void prueba_iter_insertar()	{
	lista_t *lista = lista_crear();
	printf("%s\n","INICIO PRUEBAS ITER INSERTAR, LISTA VACIA");
	lista_iter_t *iter = lista_iter_crear(lista);
	char caracter = 'c';
	print_test("\titer insertar 'c' al principio != false", lista_iter_insertar(iter, &caracter));
	print_test("\titer ver actual = 'c'", *(char*)lista_iter_ver_actual(iter) == 'c');
    lista_iter_destruir(iter);
	lista_borrar_primero(lista);
	printf("%s\n","INICIO PRUEBAS ITER INSERTAR, LISTA CON ELEMENTOS 3->2->1->0...");
	int valores[4];
	for(size_t i = 0; i < 4; i++) {
		valores[i] = (int)i;
		lista_insertar_primero(lista, &valores[i]);
	}
	iter = lista_iter_crear(lista);
	int dato = 4;
	print_test("\titer insertar 4 al principio != false", lista_iter_insertar(iter, &dato));
	print_test("\titer ver actual = 4", *(int*)lista_iter_ver_actual(iter) == 4);
	print_test("\tlista ver primero = 4", *(int*)lista_ver_primero(lista) == 4);

	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer ver actual = 3", *(int*)lista_iter_ver_actual(iter) == 3);

	double dato2 = 3.5;
	print_test("\titer insertar 3.5 al medio != false", lista_iter_insertar(iter, &dato2));
	print_test("\titer ver actual = 3.5", *(double*)lista_iter_ver_actual(iter) == 3.5);
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer avanzar en al final", !lista_iter_avanzar(iter));
	print_test("\titer esta al final", lista_iter_al_final(iter));

	dato = -1;
	print_test("\titer insertar -1 al final != false", lista_iter_insertar(iter, &dato));
	print_test("\titer ver actual = -1", *(int*)lista_iter_ver_actual(iter) == -1);
	print_test("\tlista ver ultimo = -1", *(int*)lista_ver_ultimo(lista) == -1);

	lista_destruir(lista, NULL);

	lista_iter_destruir(iter);
	print_test("\titer destruir", true);
}

void prueba_iter_borrar() {
	lista_t *lista = lista_crear();
	printf("%s\n","INICIO PRUEBAS ITER BORRAR, LISTA VACIA");
	lista_iter_t *iter = lista_iter_crear(lista);
	print_test("\titer borrar == NULL", lista_iter_borrar(iter)==NULL);
	print_test("\titer ver actual == NULL", lista_iter_ver_actual(iter) == NULL);
	lista_iter_destruir(iter);
    printf("%s\n","INICIO PRUEBAS ITER BORRAR, LISTA CON ELEMENTOS 3->2->1->0...");
	int valores[4];
	for(size_t i = 0; i < 4; i++) {
		valores[i] = (int)i;
		lista_insertar_primero(lista, &valores[i]);
	}
	iter = lista_iter_crear(lista);
	print_test("\titer borrar primer elemento (3)",*(int*)lista_iter_borrar(iter)==3);
	print_test("\titer ver actual = 2", *(int*)lista_iter_ver_actual(iter) == 2);
	print_test("\tlista ver primero = 2", *(int*)lista_ver_primero(lista) == 2);
	print_test("\titer avanzar", lista_iter_avanzar(iter));
	print_test("\titer ver actual = 1", *(int*)lista_iter_ver_actual(iter) == 1);
	print_test("\titer borrar en el medio (1)",*(int*)lista_iter_borrar(iter)==1);
	print_test("\titer ver actual = 0", *(int*)lista_iter_ver_actual(iter) == 0);
	print_test("\titer borrar el ultimo (0)",*(int*)lista_iter_borrar(iter)==0);
	print_test("\titer ver actual = NULL", lista_iter_ver_actual(iter) == NULL);
	print_test("\titer esta al final", lista_iter_al_final(iter));
	print_test("\tlista ver ultimo = 2", *(int*)lista_ver_ultimo(lista) == 2);
	lista_destruir(lista, NULL);
	lista_iter_destruir(iter);
	print_test("\titer destruir", true);
}


void prueba_crear_destruir() {
    puts("PRUEBAS CREACIÓN Y DESTRUCCIÓN");
    lista_t *lista = lista_crear();
    int valores[4] = {0, 1, 2, 3};
    // Lista 1
    print_test("\tLista crear != NULL", lista != NULL);
    print_test("\tLista borrar primero es NULL", !lista_borrar_primero(lista));
    lista_destruir(lista, NULL);
    print_test("\tLista vacía destruir con NULL", true);
    // Lista 2
    lista_t *lista2 = lista_crear();
    print_test("\tLista crear != NULL", lista2 != NULL);
    print_test("\tLista insertar", lista_insertar_primero(lista2, &valores[0]));
    print_test("\tLista insertar", lista_insertar_primero(lista2, &valores[1]));
    print_test("\tLista insertar", lista_insertar_primero(lista2, &valores[2]));
    print_test("\tLista insertar", lista_insertar_primero(lista2, &valores[3]));
    print_test("\tLargo lista es 4", lista_largo(lista2) == 4);
    print_test("\tLista no esta vacía", !lista_esta_vacia(lista2));
    print_test("\tLista insertar", lista_borrar_primero(lista2));
    print_test("\tLista insertar", lista_borrar_primero(lista2));
    print_test("\tLista insertar", lista_borrar_primero(lista2));
    print_test("\tLista insertar", lista_borrar_primero(lista2));
    print_test("\tLista esta vacía", lista_esta_vacia(lista2));
    lista_destruir(lista2, NULL);
    print_test("\tLista destruir con NULL y datos", true);
    // Lista 3
    lista_t *lista3 = lista_crear();
    char *a = malloc(sizeof(char));
    char *b = malloc(sizeof(char));
    char *c = malloc(sizeof(char));
    char *d = malloc(sizeof(char));
    char *e = malloc(sizeof(char));
    print_test("\tLista crear != NULL", lista != NULL);
    print_test("\tInsertar char = true", lista_insertar_ultimo(lista3, a));
    print_test("\tInsertar char = true", lista_insertar_ultimo(lista3, b));
    print_test("\tInsertar char = true", lista_insertar_ultimo(lista3, c));
    print_test("\tInsertar char = true", lista_insertar_ultimo(lista3, d));
    print_test("\tInsertar char = true", lista_insertar_ultimo(lista3, e));
    lista_destruir(lista3, free);
    print_test("\tLista destruir con free", true);
}


void prueba_lista_vacia() {
    puts("PRUEBAS CON LISTA VACÍA");
    int a = 4;
    lista_t *lista = lista_crear();
    print_test("\tLista crear != NULL", lista!=NULL);
    print_test("\tInsertar primero", lista_insertar_primero(lista, &a));
    print_test("\tVer primero == Ver ultimo", lista_ver_primero(lista) == lista_ver_ultimo(lista));
    print_test("\tLargo no es 0", lista_largo(lista));
    print_test("\tBorrar primero != NULL", lista_borrar_primero(lista));
    print_test("\tBorrar primero es NULL", !lista_borrar_primero(lista));
    print_test("\tLargo es 0", !lista_largo(lista));
    print_test("\tLista esta vacía es true", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
}

void prueba_lista_null() {
    puts("PRUEBAS CON NULL");
    lista_t *lista = lista_crear();
    print_test("\tLista crear != NULL", lista != NULL);
    print_test("\tInserto NULL primero", lista_insertar_primero(lista, NULL));
    print_test("\tInserto NULL ultimo", lista_insertar_ultimo(lista, NULL));
    print_test("\tLista ver primero es NULL", !lista_ver_primero(lista));
    print_test("\tLista ver ultimo es NULL", !lista_ver_ultimo(lista));
    print_test("\tLista no está vacía", !lista_esta_vacia(lista));
    print_test("\tLargo de lista = 2", lista_largo(lista) == 2);
    lista_destruir(lista, NULL);
    printf("\tLista destruir con datos NULL"); print_test("", true);
}

void prueba_volumen() {
    puts("PRUEBA VOLUMEN");
    lista_t *lista = lista_crear();
    size_t i;
    int datos1[200];
    for (i = 0; i < 200; i++) {
        datos1[i] = (int)i;
        lista_insertar_primero(lista, &datos1[i]);
    }
    while(--i && i > 194) {
        int *aux = lista_borrar_primero(lista);
        print_test("\tInvariante lista ver primero", *aux == datos1[i]);
    }
    for(i = 0; i < 200; i++) {
        lista_insertar_ultimo(lista, &datos1[i]);
    }
    while(--i && i > 194) {
        lista_borrar_primero(lista);
        int *aux = lista_ver_ultimo(lista);
        print_test("\tInvariante lista ver ultimo", *aux == datos1[199]);
    }
    print_test("\tLista largo = 390", lista_largo(lista) == 390);
    lista_destruir(lista, NULL);
    printf("\tLista destruir con datos"); print_test("", true);
}


bool sumar(void *dato, void *extra) {
    if (!dato)
        return false;
    *(int*) extra += *(int*) dato;
    return true;
}

bool contar_pares(void *dato, void *extra) {
    if (!dato)
        return false;
    if (*(int*)dato % 2 == 0)
        *(int*) extra += 1;
    return true;
}

bool menores_ocho(void *dato, void *extra) {
    if (!dato)
        return false;
    if (*(int*)dato < 8)
        *(int*) extra += 1;
    return true;
}

bool imprimir(void *dato, void *extra) {
    if (!dato)
        return false;
    printf("\t%i", *(int*)dato);
    return true;
}

void pruebas_de_iterador_interno() {
    puts("PRUEBAS ITERADOR INTERNO");
    lista_t *lista = lista_crear();
    int datos[5] = {1, 4, 8, 12, 15};
    int suma = 0, pares = 0, menores = 0;
    print_test("\tLista insertar", lista_insertar_primero(lista, &datos[0]));
    print_test("\tLista insertar", lista_insertar_primero(lista, &datos[1]));
    print_test("\tLista insertar", lista_insertar_primero(lista, &datos[2]));
    print_test("\tLista insertar", lista_insertar_primero(lista, &datos[3]));
    print_test("\tLista insertar", lista_insertar_primero(lista, &datos[4]));
    lista_iterar(lista, imprimir, NULL);
    print_test("\n\tDatos: ", true);
    lista_iterar(lista, sumar, &suma);
    print_test("\tSuma = 40", suma == 40);
    lista_iterar(lista, contar_pares, &pares);
    print_test("\tPares = 3", pares == 3);
    lista_iterar(lista, menores_ocho, &menores);
    print_test("\tMenores que 8 = 2", menores == 2);
    lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante() {
	prueba_crear_destruir();
    prueba_lista_vacia();
    prueba_lista_null();
    prueba_volumen();
    pruebas_de_iterador_interno(); 
    prueba_iter_crear_destruir();
    prueba_iter_insertar();
	prueba_iter_borrar();
    prueba_iter_volumen();
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
  pruebas_lista_estudiante();
  return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
