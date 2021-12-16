#include "usuario.h"
#include "hash.h"
#include "heap.h"

struct usuario {
    char* nombre;
    int numero_usuario;
    heap_t* posts;
};

struct usuario_post {
  post_t *post;
  int prioridad;
};

static usuario_post_t *usuario_post_crear(post_t *post, usuario_t *usuario) {
    usuario_post_t *usuario_post = malloc(sizeof(usuario_post_t));
    if (!usuario_post) return NULL;
    usuario_post->post = post;
    int numero_autor = post_numero_autor(post);
    usuario_post->prioridad = (int)abs(numero_autor - usuario->numero_usuario);
    return usuario_post;
}

static void usuario_post_destruir(void *usuario_post) {
    free((usuario_post_t *)usuario_post);
}

// x e y son dos posts con sus respectivas prioridades para el usuario. 
// (-1) convierte heap de maximos en heap de minimos
static int prioridad_cmp(const void *x, const void *y) {
    usuario_post_t* usuario1 = (usuario_post_t*) x;
    usuario_post_t* usuario2 = (usuario_post_t*) y;
    int prioridad = (-1)*(usuario1->prioridad - usuario2->prioridad);
    if (prioridad == 0)
        return (-1)*(int)(post_id(usuario1->post) - post_id(usuario2->post));
    else return prioridad;
}

usuario_t *usuario_crear(char *nombre, int numero_usuario) {
    usuario_t *usuario = malloc(sizeof(usuario_t));
    if(!usuario) return NULL;
    
    usuario->numero_usuario = numero_usuario;
    usuario->nombre = malloc(sizeof(char) * strlen(nombre) + 1);
    if(!usuario->nombre) {
      free(usuario);
      return NULL;
    }
    strcpy(usuario->nombre, nombre);
    usuario->posts = heap_crear(prioridad_cmp);
    if(!usuario->posts) {
      free(usuario->nombre);
      free(usuario);
      return NULL;
    }
    return usuario;
}

void usuario_destruir(void *usuario) {
    free(((usuario_t *)usuario)->nombre);
    heap_destruir(((usuario_t *)usuario)->posts, usuario_post_destruir);
    free(usuario);
}

char *usuario_nombre(usuario_t *usuario) {
    return usuario->nombre;
}

int usuario_ver_numero(usuario_t *usuario) {
    return usuario->numero_usuario;
}


post_t *usuario_ver_siguiente_post(usuario_t *usuario) {
    usuario_post_t *usuario_post =  heap_desencolar(usuario->posts);
    post_t *post = usuario_post->post;
    usuario_post_destruir(usuario_post);
    return post;
}

bool usuario_agregar_post(usuario_t *usuario, post_t *post) {
    usuario_post_t *usuario_post = usuario_post_crear(post, usuario);
    return heap_encolar(usuario->posts, usuario_post);
}

bool usuario_hay_posts(usuario_t *usuario) {
    return !heap_esta_vacio(usuario->posts);
}
