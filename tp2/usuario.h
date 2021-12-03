#ifndef USUARIO_H
#define USUARIO_H

#include "librerias.h"
#include "post.h"

typedef struct usuario usuario_t;

// Contiene un post y una prioridad relativa al usuario en cuestión
typedef struct usuario_post usuario_post_t;

/* Crea una estructura usuario_t
* Pre: recibe un nombre de usuario válido y un numero de usuario >= 0
* Post: devuelve una estructura usuario en memoria nueva creada e inicializada
*/
usuario_t *usuario_crear(char *nombre, int numero_usuario);

/* Destruye la estructura liberando la memoria pedida
* Pre: Recibe una estructura usuario como void* creada e inicializada
* Post: La estructura usuario fue destruída
*/
void usuario_destruir(void *usuario);

/* Devuelve el numero de usuario propio al usuario recibido
* Pre: La estructura usuario fue creada e inicializada
*/
int usuario_ver_numero(usuario_t *usuario);

/* Devuelve el nombre de usuario de la estructura recibida
* Pre: La estructura usuario fue creada e inicializada
*/
char *usuario_nombre(usuario_t *usuario);

/* Agrega el post al resto de los que posee el usuario recibido
* Pre: Las estructuras usuario y post fueron creadas e inicializadas
* Post: El post fue agregado dentro de la estructura usuario y devuelve true.  * se devuelve false en caso de fallo de memoria
*/
bool usuario_agregar_post(usuario_t *usuario, post_t *post);

/* Indica si hay posts sin ver dentro del usuario recibido
* Pre: La estructura usuario fue creada e inicializada
*/
bool usuario_hay_posts(usuario_t *usuario);

/* Devuelve el post_t dentro de la estructura auxiliar usuario_post_t
* Pre: la estructura usuario_post_t fue creada e inicializada (con un post)
*/
post_t *ver_post(usuario_post_t *post);

/* Devuelve el proximo post en el feed del usuario recibido
* Pre: La estructura usuario fue creada e inicializada
* Post: El post ya no pertenece al feed de usuario y se libero la memoria     * propia de la estructura usuario_post_t (no de post_t perteneciente a la     * respectiva estructura)
*/
post_t *usuario_ver_siguiente_post(usuario_t *usuario);


#endif  // USUARIO_H