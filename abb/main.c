#include "testing.h"

#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_abb_estudiante(void);

#ifndef CORRECTOR
int main(int argc, char *argv[]) {

    printf("\n~~~ PRUEBAS ESTUDIANTE ~~~\n");
    pruebas_abb_estudiante();

    return failure_count() > 0;
}
#endif
