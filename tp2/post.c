#include "post.h"
#include "abb.h"

#define MSG_ERROR_POST "Error: Post inexistente o sin likes"
#define MSG_LIKE_POST "Post likeado"

struct post {
	size_t id;
    char *autor;
    char *contenido;
    int numero_autor;
	abb_t* usuarios_likes;
	size_t cantidad_likes;
};

post_t *post_crear(size_t id, char *autor, int numero_autor, const char *contenido, size_t len) {
    post_t *post = malloc(sizeof(post_t));
    if (!post) return NULL;

    post->autor = malloc(sizeof(char)*strlen(autor) + 1);
    if(!post->autor) {
        free(post);
        return NULL;
    }
    strcpy(post->autor, autor);
    post->numero_autor = numero_autor;
    post->id = id;
    post->contenido = malloc(sizeof(char)*len + 1);
    if(!post->contenido) {
        free(post->autor);
        free(post);
        return NULL;
    }
    strcpy(post->contenido, contenido);
    post->usuarios_likes = abb_crear(strcmp, NULL);
    post->cantidad_likes = 0;
    return post;
}

void post_destruir(void *post) {
    free(((post_t*)post)->contenido);
    free(((post_t*)post)->autor);
    abb_destruir(((post_t*)post)->usuarios_likes);
    free((post_t*)post);
}

char *post_ver_autor(post_t *post) {
    return post->autor;
}

size_t post_id(post_t *post) {
    return post->id;
}

char *post_ver_contenido(post_t *post) {
    return post->contenido;
}

int post_numero_autor(post_t *post) {
    return post->numero_autor;
}

size_t post_cant_likes(post_t *post) {
    return post->cantidad_likes;
}

void post_dar_like(char *usuario, post_t *post) {
    if(abb_pertenece(post->usuarios_likes, usuario)) {
        fprintf(stdout, "%s\n", MSG_LIKE_POST);
        return;
    }
    abb_guardar(post->usuarios_likes, usuario, NULL);
    post->cantidad_likes ++;
    fprintf(stdout, "%s\n", MSG_LIKE_POST);
}

void post_mostrar_likes(post_t *post) {
    if (post->cantidad_likes == 0) {
        fprintf(stdout, "%s\n", MSG_ERROR_POST);
        return;
    }
    fprintf(stdout, "El post tiene %zd likes:\n", post->cantidad_likes);
    abb_iter_t *abb_iter = abb_iter_in_crear(post->usuarios_likes);
    if (!abb_iter) return;
    while (!abb_iter_in_al_final(abb_iter)) {
        fprintf(stdout, "\t%s\n", abb_iter_in_ver_actual(abb_iter));
        abb_iter_in_avanzar(abb_iter);
    }
    abb_iter_in_destruir(abb_iter);
}
