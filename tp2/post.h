#ifndef POST_H
#define POST_H

#include "librerias.h"

typedef struct post post_t;

/* Crea una estructura post_t
* Pre: recibe un id >= 0, un autor válido, un numero_autor >= 0, un contenido  * válido y su longitud
* Post: devuelve una estructura post en memoria nueva creada e inicializada
*/
post_t *post_crear(size_t id, char *autor, int numero_autor, const char *contenido, size_t len);

/* Destruye la estructura liberando la memoria pedida
* Pre: La estructura post fue creada e inicializada
* Post: La estructura post fue destruída
*/
void post_destruir(void *post);

/* Devuelve el nombre del autor del post recibido
* Pre: La estructura post fue creada e inicializada
* Post: Devuelve una cadena válida con el nombre del autor
*/
char *post_ver_autor(post_t *post);

/* Devuelve el contenido del post recibido (contenido = texto escrito por el   * autor)
* Pre: La estructura post fue creada e inicializada
* Post: Devuelve una cadena válida con el contenido del post
*/
char *post_ver_contenido(post_t *post);

/* Devuelve la cantidad de likes del post recibido
* Pre: La estructura post fue creada e inicializada
*/
size_t post_cant_likes(post_t *post);

/* Devuelve el ID del post recibido (ID es un número único propio de cada post)
* Pre: La estructura post fue creada e inicializada
*/
size_t post_id(post_t *post);

/* Devuelve el numero_autor del post recibido (numero_autor es un número único propio de cada autor que sirve para calculo de prioridad)
* Pre: La estructura post fue creada e inicializada
*/
int post_numero_autor(post_t *post);

/* Aumenta la cantidad de likes del post y agrega al usuario que lo likeo a la * lista
* Pre: Las estructuras post y usuario fueron creadas e inicializadas
* Post: Modifica la estructura post en caso de que el usuario no haya dado    * like a la publicacion antes e imprime por salida estándar "Post Likeado".   * Caso contrario solo imprime la salida.
*/
void post_dar_like(char *usuario, post_t *post);

/* Imprime por salida estándar los nombres de los usuarios que hayan dado like * al post
* Pre: La estructura post fue creada e inicializada
*/
void post_mostrar_likes(post_t *post);

#endif  // POST_H