#include "testing.h"

#include <stdio.h>
#include <stdlib.h>

void pruebas_heap_estudiante(void);

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

#ifndef CORRECTOR
int main(int argc, char *argv[]) {
    printf("~~~ PRUEBAS ESTUDIANTE ~~~\n");
    pruebas_heap_estudiante();

    return failure_count() > 0;
}
#endif
