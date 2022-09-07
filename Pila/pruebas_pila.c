#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#define PRUEBA_VOLUMEN_CANT 1000


/* Apila elementos de un vector y si algo falla en el proceso devuelve false */
static bool apilar_elementos(pila_t *pila, int elementos[], size_t cant){
    for (int i = 0; i < cant; i++) {
        if(!pila_apilar(pila,&(elementos[i]))){
            return false;
        }
    }
    return true;
}

static bool aux_prueba_volumen(pila_t *pila,int* vector,size_t cant){
    int *nuevo_tope,*tope_actual;
    for (int i = (PRUEBA_VOLUMEN_CANT-1); i >= 0; i--) {
        tope_actual=(int*)pila_ver_tope(pila);
        nuevo_tope=(int*)pila_desapilar(pila);
        if(*tope_actual!=vector[i] || *nuevo_tope!=vector[i]){ //reviso si esta bien el tope antes y despues de desapilar
            return false;

        }
    }
    if(pila_ver_tope(pila)!=NULL||pila_desapilar(pila)!=NULL){ // caso especial por el ultimo elemento
        return false;
    }
    return true;
}

static void prueba_pila_vacia(void) {
    printf("\nINICIO DE PRUEBAS CON PILA VACIA\n");
    pila_t *pila = pila_crear();

    print_test("Pila esta vacia", pila_esta_vacia(pila));
    print_test("Ver tope pila vacia", pila_ver_tope(pila)==NULL);
    print_test("Desapilar pila vacia", pila_desapilar(pila)==NULL);
    pila_destruir(pila);
}

static void prueba_apilar_desapilar(void) {
    printf("\nINICIO DE PRUEBAS DE APILAR Y DESAPILAR\n");
    pila_t *pila = pila_crear();
    int vector1[] = { 1, 2, 3, 4, 5 };

    print_test("Apilar elementos vector [1,2,3,4,5]", apilar_elementos(pila,vector1,5));
    print_test("Ver tope (5)", *((int*)pila_ver_tope(pila))==5);
    print_test("Desapilar (5)", *((int*)pila_desapilar(pila))==5);
    print_test("Ver tope (4)", *((int*)pila_ver_tope(pila))==4);
    print_test("Desapilar (4)", *((int*)pila_desapilar(pila))==4);
    print_test("Desapilar (3)", *((int*)pila_desapilar(pila))==3);
    print_test("Desapilar (2)", *((int*)pila_desapilar(pila))==2);
    print_test("Desapilar (1)", *((int*)pila_desapilar(pila))==1);
    print_test("Ver tope (pila vacia)", pila_ver_tope(pila)==NULL);
    print_test("Desapilar pila vacia", pila_desapilar(pila)==NULL);
    pila_destruir(pila);
}

static void prueba_pila_volumen(void) {
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");
    pila_t *pila = pila_crear();

    int *vector2=malloc(sizeof(int)*PRUEBA_VOLUMEN_CANT);
    for (int i = 0; i < PRUEBA_VOLUMEN_CANT; i++) {
        vector2[i]=i;
    }
    print_test("Apilar elementos vector [1,2,3,4,...,PRUEBA_VOLUMEN_CANT]", apilar_elementos(pila,vector2,PRUEBA_VOLUMEN_CANT));
    print_test("Desapila y ver tope uno por uno", aux_prueba_volumen(pila,vector2,PRUEBA_VOLUMEN_CANT));
    print_test("Apilar elementos vector [1,2,3,4,...,PRUEBA_VOLUMEN_CANT]", apilar_elementos(pila,vector2,PRUEBA_VOLUMEN_CANT));
    print_test("Desapila y ver tope uno por uno", aux_prueba_volumen(pila,vector2,PRUEBA_VOLUMEN_CANT));
    print_test("Ver tope (pila vacia)", pila_ver_tope(pila)==NULL);
    print_test("Desapilar (pila vacia)", pila_desapilar(pila)==NULL);
    pila_destruir(pila);
    free(vector2);
}

static void prueba_apilar_NULL(void) {
    printf("\nINICIO DE PRUEBA APILAR NULL\n");
    pila_t *pila = pila_crear();

    print_test("Apilar NULL", pila_apilar(pila,NULL));
    print_test("Ver tope NULL", pila_ver_tope(pila)==NULL);
    print_test("Desapilar pila", pila_desapilar(pila)==NULL);
    pila_destruir(pila);
}


void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    prueba_apilar_desapilar();
    prueba_pila_volumen();
    prueba_apilar_NULL();
}



/*
 * Función main() que llama a la función de pruebas.
 */


#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
