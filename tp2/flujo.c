#include "flujo.h"
#include "post.h"

#define LARGO_SUFICIENTE_PARA_ID_INT 11
#define MSG_ERROR_LOGGED "Error: Ya habia un usuario loggeado"
#define MSG_ERROR_NOT_LOGGED "Error: no habia usuario loggeado"
#define MSG_ERROR_NONEXISTENT_USER "Error: usuario no existente"
#define MSG_LOGIN "Hola "
#define MSG_LOGOUT "Adios"
#define MSG_POST_PUBLISHED "Post publicado"
#define MSG_ERROR_NOT_LOGGED_OR_NO_POSTS "Usuario no loggeado o no hay mas posts para ver"
#define MSG_ERROR_NOT_LOGGED_OR_NONEXISTENT_POST "Error: Usuario no loggeado o Post inexistente"

#define MSG_ERROR_NONEXISTENT_POST_OR_WITHOUT_LIKES "Error: Post inexistente o sin likes"

struct estado {
    bool login;
    usuario_t *logueado;
    hash_t *posts;
    size_t cant_posts;
    hash_t *usuarios;
};

estado_t *estado_crear(hash_t *usuarios) {
    estado_t *estado = malloc(sizeof(estado_t));
    if (!estado) return NULL;
    estado->login = false;
    estado->logueado = NULL;
    estado->posts = hash_crear(post_destruir);
    estado->cant_posts = 0;
    estado->usuarios = usuarios;
    return estado;
}

void estado_destruir(estado_t *estado) {
    hash_destruir(estado->posts);
    free(estado);
}

estado_t *iniciar_programa(FILE *archivo_usuarios) {
    size_t bufsize = 0;
    char *buffer = NULL;
	hash_t *usuarios = hash_crear(usuario_destruir);
    estado_t *estado = estado_crear(usuarios);
	
    ssize_t leidos;
    int linea = 0;
	while ((leidos = getline(&buffer, &bufsize, archivo_usuarios)) != -1) {
        buffer[leidos-1] = '\0';
        usuario_t *usuario = usuario_crear(buffer, linea);
		    if (!hash_guardar(usuarios, buffer, usuario)) {
			      hash_destruir(usuarios);
                  estado_destruir(estado);
                  free(buffer);
                  return NULL;
		    }
        linea ++;
	}

    estado_agregar_usuarios(estado, usuarios);
    free(buffer);
    return estado;
}

bool estado_login(estado_t *estado) {
    return estado->login;
}

usuario_t *estado_logueado(estado_t *estado) {
    return estado->logueado;
}

size_t estado_cant_posts(estado_t *estado) {
    return estado->cant_posts;
}

void estado_agregar_usuarios(estado_t *estado, hash_t *usuarios) {
    estado->usuarios = usuarios;
}

hash_t *estado_usuarios(estado_t *estado) {
    return estado->usuarios;
}

void login(estado_t *estado) {    
    char *buffer = NULL;
    size_t bufsize= 0;
    ssize_t leidos = getline(&buffer, &bufsize, stdin);
    buffer[leidos-1] = '\0';
    if(estado_login(estado)) {
        fprintf(stdout, "%s\n", MSG_ERROR_LOGGED);
        free(buffer);
        return;
    }
    usuario_t *usuario;
    if(!(usuario = hash_obtener(estado->usuarios, buffer))) {
        fprintf(stdout, "%s\n", MSG_ERROR_NONEXISTENT_USER);
        free(buffer);
        return;
    }

    estado->login = true;
    estado->logueado = usuario;
    fprintf(stdout, "%s%s\n", MSG_LOGIN, usuario_nombre(usuario));
    free(buffer);
    return;
}

void logout(estado_t *estado) {
    if (!estado_login(estado)) {
        fprintf(stdout, "%s\n", MSG_ERROR_NOT_LOGGED);
        return;
    }
    estado->login = false;
    estado->logueado = NULL;
    fprintf(stdout, "%s\n", MSG_LOGOUT);
}

static bool publicar_(estado_t *estado, usuario_t *autor, char *contenido, size_t len) {
    post_t *post = post_crear(estado->cant_posts, usuario_nombre(autor), usuario_ver_numero(autor), contenido, len);
    if (!post)
        return false;
    hash_iter_t *iter = hash_iter_crear(estado->usuarios);
    if (!iter) 
        return false;
    while(!hash_iter_al_final(iter)) {
        usuario_t *usuario = hash_obtener(estado->usuarios, hash_iter_ver_actual(iter));
        if (strcmp(usuario_nombre(usuario), usuario_nombre(estado->logueado))) {
            if(!usuario_agregar_post(usuario, post)) {
                hash_iter_destruir(iter);
                post_destruir(post);
                return false;
            }
        }
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    char str[LARGO_SUFICIENTE_PARA_ID_INT];
    sprintf(str, "%i", (int)estado->cant_posts);
    hash_guardar(estado->posts, str, post);
    estado->cant_posts ++;
    return true; 
}

void publicar(estado_t *estado) {
    usuario_t *autor = estado_logueado(estado);
    char *buffer = NULL;
    size_t bufsize = 0;
    if(getline(&buffer, &bufsize, stdin) == -1) {
        free(buffer);
        return;
    }
    if (!estado_login(estado)){
        fprintf(stdout, "%s\n", MSG_ERROR_NOT_LOGGED);
        free(buffer);
        return;
    }
    if(!publicar_(estado, autor, buffer, strlen(buffer))) {
        free(buffer);
        return;
    }
    fprintf(stdout, "%s\n", MSG_POST_PUBLISHED);
    free(buffer);

    return;
}

void ver_siguiente_feed(estado_t *estado) {
    if(!estado_login(estado) || !usuario_hay_posts(estado->logueado)) {
        fprintf(stdout, "%s\n", MSG_ERROR_NOT_LOGGED_OR_NO_POSTS);
        return;
    }
    usuario_t *usuario = estado->logueado;
    post_t *post = usuario_ver_siguiente_post(usuario);
    fprintf(stdout, "Post ID %zd\n%s dijo: %sLikes: %zd\n", post_id(post), post_ver_autor(post), post_ver_contenido(post), post_cant_likes(post));
}

static post_t *obtener_post(estado_t *estado) {
    char *id = NULL;
    size_t buffsize = 0;
    ssize_t leidos = getline(&id, &buffsize, stdin);
    id[leidos-1] = '\0';
    post_t *post = hash_obtener(estado->posts, id);
    if (!post){
        free(id);
        return NULL;
    }
    free(id);
    return post;
}

void likear_post(estado_t *estado) {
    post_t *post = obtener_post(estado);
    if(!estado_login(estado)) {
        fprintf(stdout, "%s\n", MSG_ERROR_NOT_LOGGED_OR_NONEXISTENT_POST);
        return;
    }
    if (!post) {
        fprintf(stdout, "%s\n", MSG_ERROR_NOT_LOGGED_OR_NONEXISTENT_POST);
        return;
    }
    post_dar_like(usuario_nombre(estado->logueado), post);
}

void mostrar_likes(estado_t *estado) {
    post_t *post = obtener_post(estado);
    if (!post) {
        fprintf(stdout, "%s\n", MSG_ERROR_NONEXISTENT_POST_OR_WITHOUT_LIKES);
        return;
    }
    post_mostrar_likes(post);
    return;
}

