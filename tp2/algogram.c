#define _POSIX_C_SOURCE 200809L
#include "flujo.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG_ERROR_PARAMETERS "Falta archivo de usuarios"
#define MSG_ERROR_FILE "Nombre de archivo incorrecto o ruta incorrecta"

#define N_FUNCIONES 6
#define N_PARAMETROS 2
/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/
FILE *validar_parametros(int argc, char *argv[]) {
    if(argc != N_PARAMETROS){
        fprintf(stderr, "%s\n", MSG_ERROR_PARAMETERS);
        return NULL;
    }
    FILE *archivo_usuarios = fopen(argv[1],"r");
    if(!archivo_usuarios){
        fprintf(stderr, "%s\n", MSG_ERROR_FILE);
        return NULL;
    }
    return archivo_usuarios;
}

hash_t *menu_funciones(char **clave, algogram_funcion_t *funciones, size_t n) {
    hash_t *hash = hash_crear(NULL);
    if (!hash)
        return NULL;
    for (size_t i = 0; i < n; i++) {
        hash_guardar(hash, clave[i], &funciones[i]);
    }
    return hash;
}


int main(int argc, char *argv[]) {
    char *claves[] = {"login", "logout", "publicar", "ver_siguiente_feed", "mostrar_likes", "likear_post"};
    size_t n = N_FUNCIONES;
    algogram_funcion_t funciones[] = {login, logout, publicar, ver_siguiente_feed, mostrar_likes, likear_post};

    hash_t *menu = menu_funciones(claves, funciones, n);

    FILE *usuarios = validar_parametros(argc, argv);
    if (!usuarios)
        return EXIT_FAILURE;

    estado_t *estado = iniciar_programa(usuarios);
    fclose(usuarios);
    
    ssize_t leidos;
    size_t bufsize = 0;
    char *buffer = NULL;
	while ((leidos = getline(&buffer,&bufsize,stdin) != -1)) {
        buffer[strlen(buffer)-1] = '\0';
        algogram_funcion_t funcion = *(algogram_funcion_t*)hash_obtener(menu, buffer);
        (*funcion)(estado);
	}
    
    free(buffer);
    hash_destruir(menu);
    hash_destruir(estado_usuarios(estado));
    estado_destruir(estado);
}