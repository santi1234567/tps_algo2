#ifndef ALGOGRAM_H
#define ALGOGRAM_H

#include "librerias.h"
#include "hash.h"
#include "usuario.h"

typedef struct estado estado_t;
typedef void (*algogram_funcion_t) (estado_t *);

/* Crea un estado_t 
*/
estado_t *estado_crear();

/* Destruye la estructura liberando la memoria pedida
* Pre: La estructura estado fue creada e inicializada
* Post: La estructura estado fue destruída
*/
void estado_destruir(estado_t *estado);

/* Devuelve un bool indicando si existe un usuario logueado en estado
* Pre: La estructura estado fue creada e inicializada
*/
bool estado_login(estado_t *estado);

/* Devuelve el usuario que esta logueado al momento. Si no hay usuario devuelve NULL 
* Pre: La estructura estado fue creada e inicializada
*/
usuario_t *estado_logueado(estado_t *estado);

/* Devuelve la cantidad de posteos realizados desde la ejecución del programa, 0 si no * se realizó ninguno
* Pre: La estructura estado fue creada e inicializada
*/
size_t estado_cant_posts(estado_t *estado);

/* Agrega el hash de usuarios recibido a la estructura estado.
* Pre: Las estructuras estados y hash usuarios fueron creadas e inicializadas
* Post: Modifica la estructura estado agregando los usuarios
*/
void estado_agregar_usuarios(estado_t *estado, hash_t *usuarios);

/* Devuelve en un hash los usuarios pertenecientes a la estructura estado
* Pre: La estructura estado fue creada e inicializada
*/
hash_t *estado_usuarios(estado_t *estado);

/* Lee un nombre de la entrada estándar y modifica la estructura estado en caso de que * no se haya hecho un login antes. En caso de que haya un login previo imprime un error * por salida estándar.
* Pre: La estructura estado fue creada e inicializada
* Post: Se loguea el estado leido por entrada estándar en caso de que no haya un login * previo
*/
void login(estado_t *estado);

/* Desloguea al usuario presente al momento en estado. En caso de que no haya un login * previo imprime un error por salida estándar
* Pre: La estructura estado fue creada e inicializada
* Post: modifica la estructura estado, quitando el login del usuario previo
*/
void logout(estado_t *estado);

/* Lee un contenido por entrada estándar, crea y realiza una publicación
* Pre: La estructura estado fue creada e inicializada
* Post: se creó un usuaro_post_t, se lo añadió a cada usuario segun su prioridad *relativa, y se agregó un post_t al resto de posts en estado
*/
void publicar(estado_t *estado);

/* Muestra por salida estándar el siguiente post en orden de prioridad relativa al * usuario logueado al momento.
* Pre: La estructura estado fue creada e inicializada
*/
void ver_siguiente_feed(estado_t *estado);

/* Realiza un like sobre cierto post por parte del usuario logueado
* Pre: La estructura estado fue creada e inicializada
* Post: aumenta en 1 la cantidad de likes del post cuyo ID es leido por entrada         * estándar y se agrega al usuario al de usuarios que likearon la publicación
*/
void likear_post(estado_t *estado);

/* Imprime por salida estandar la cantidad de likes y aquellas personas que le dieron  * like a la publicación cuyo ID es ingresado por entrada estandar. En caso de que no   * exista el post con el ID correspondiente, se imprime un error por salida estándar.
*/
void mostrar_likes(estado_t *estado);

/* Crea un estado, lee usuarios del archivo recibido y los carga sobre la     * estructura.
* Pre: recibe un archivo de texto abierto en modo lectura
* Post: recibe una estructura estado creada e inicializada, con los usuarios  * leidos desde el archivo
*/
estado_t *iniciar_programa(FILE *archivo_usuarios);

#endif  // ALGOGRAM_H
