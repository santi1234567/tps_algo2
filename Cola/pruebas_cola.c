#include "cola.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#define PRUEBA_VOLUMEN_CANT 1000
#define PRUEBA_FREE_CANT 100

/* Encola elementos de un vector y si algo falla en el proceso devuelve false */

static bool encolar_elementos(cola_t *cola, int elementos[], size_t cant){
    for (int i = 0; i < cant; i++) {
        if(cola_encolar(cola,&(elementos[i]))==false){
            return false;
        }

    }
    return true;
}

static bool aux_prueba_volumen(cola_t *cola,int* vector,size_t cant){
    int *nuevo_primero,*primero_actual;
    for (int i = 0; i < PRUEBA_VOLUMEN_CANT; i++) {
        primero_actual=(int*)cola_ver_primero(cola);
        nuevo_primero=(int*)cola_desencolar(cola);
        if(*primero_actual!=vector[i] || *nuevo_primero!=vector[i]){ //reviso si esta bien el primero antes y despues de Desencolar
            return false;
        }
    }
    if(cola_ver_primero(cola)!=NULL||cola_desencolar(cola)!=NULL){ // caso especial por el ultimo elemento
        return false;
    }
    return true;
}

static void prueba_cola_vacia(void) {
    printf("\nINICIO DE PRUEBAS CON COLA VACIA\n");
    cola_t *cola = cola_crear();

    print_test("Cola esta vacia", cola_esta_vacia(cola));
    print_test("Ver primero cola vacia", cola_ver_primero(cola)==NULL);
    print_test("Desencolar cola vacia", cola_desencolar(cola)==NULL);
    cola_destruir(cola,NULL);
}

static void prueba_encolar_desencolar(void) {
    printf("\nINICIO DE PRUEBAS DE ENCOLAR Y DESENCOLAR\n");
    cola_t *cola = cola_crear();
    int vector1[] = { 5, 4, 3, 2, 1 };

    print_test("encolar elementos vector [5, 4, 3, 2, 1 ]", encolar_elementos(cola,vector1,5));
    print_test("Ver primero (5)", *((int*)cola_ver_primero(cola))==5);
    print_test("Desencolar (5)", *((int*)cola_desencolar(cola))==5);
    print_test("Ver primero (4)", *((int*)cola_ver_primero(cola))==4);
    print_test("Desencolar (4)", *((int*)cola_desencolar(cola))==4);
    print_test("Desencolar (3)", *((int*)cola_desencolar(cola))==3);
    print_test("Desencolar (2)", *((int*)cola_desencolar(cola))==2);
    print_test("Desencolar (1)", *((int*)cola_desencolar(cola))==1);
    print_test("Ver primero (cola vacia)", cola_ver_primero(cola)==NULL);
    print_test("Desencolar cola vacia", cola_desencolar(cola)==NULL);
    cola_destruir(cola,NULL);
    cola = cola_crear();

    print_test("encolar elementos vector [5, 4, 3, 2, 1 ]", encolar_elementos(cola,vector1,5));
    print_test("Ver primero (5)", *((int*)cola_ver_primero(cola))==5);
    print_test("Desencolar (5)", *((int*)cola_desencolar(cola))==5);
    print_test("Ver primero (4)", *((int*)cola_ver_primero(cola))==4);
    print_test("Desencolar (4)", *((int*)cola_desencolar(cola))==4);
    print_test("Desencolar (3)", *((int*)cola_desencolar(cola))==3);
    print_test("Desencolar (2)", *((int*)cola_desencolar(cola))==2);
    print_test("Desencolar (1)", *((int*)cola_desencolar(cola))==1);
    print_test("Ver primero (cola vacia)", cola_ver_primero(cola)==NULL);
    print_test("Desencolar cola vacia", cola_desencolar(cola)==NULL);
    cola_destruir(cola,NULL);

}

static void prueba_cola_volumen(void) {
    printf("\nINICIO DE PRUEBAS DE VOLUMEN\n");
    cola_t *cola = cola_crear();
    print_test("Cola esta vacia", cola_esta_vacia(cola));

    int *vector2=malloc(sizeof(int)*PRUEBA_VOLUMEN_CANT);

    for (int i = 0; i < PRUEBA_VOLUMEN_CANT; i++) {
        vector2[i]=PRUEBA_VOLUMEN_CANT-i-1;
    }
    print_test("encolar elementos vector [PRUEBA_VOLUMEN_CANT,....,4,3,2,1]", encolar_elementos(cola,vector2,PRUEBA_VOLUMEN_CANT));
    print_test("Desencolar y ver primero uno por uno", aux_prueba_volumen(cola,vector2,PRUEBA_VOLUMEN_CANT));
    print_test("encolar elementos vector [PRUEBA_VOLUMEN_CANT,....,4,3,2,1] y destruir", encolar_elementos(cola,vector2,PRUEBA_VOLUMEN_CANT));
    cola_destruir(cola,NULL);
    free(vector2);
}


static void prueba_encolar_NULL(void) {
    printf("\nINICIO DE PRUEBA ENCOLAR NULL\n");
    cola_t *cola = cola_crear();

    print_test("Encolar NULL", cola_encolar(cola,NULL));
    print_test("Ver primero NULL", cola_ver_primero(cola)==NULL);
    print_test("Desencolar cola", cola_desencolar(cola)==NULL);
    cola_destruir(cola,NULL);
}



static void prueba_destruir_cola_free(void){
    printf("\nINICIO DE PRUEBA DESTRUIR COLA free()\n");
    cola_t *cola = cola_crear();
    bool estado=true;
    int **aux;
    aux=malloc(sizeof(int*)*PRUEBA_FREE_CANT);
    for (size_t i = 0; i < PRUEBA_FREE_CANT; i++) {
        aux[i]=malloc(sizeof(int)*(i+1));
        if(aux[i]==NULL){
            estado=false;
            break;
        }
        estado=cola_encolar(cola,aux[i]);
        if(!estado){
            break;
        }
    }
    print_test("Encolar punteros y destruir", estado);
    cola_destruir(cola,free);
    free(aux);
}

void pruebas_cola_estudiante() {
    prueba_cola_vacia();
    prueba_encolar_desencolar();
    prueba_cola_volumen();
    prueba_encolar_NULL();
    prueba_destruir_cola_free();
}



/*
 * Función main() que llama a la función de pruebas.
 */


#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
