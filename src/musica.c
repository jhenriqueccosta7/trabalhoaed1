#include "musica.h"
#include <stdlib.h>
#include <string.h>

t_musica *musica_criar(int id, const char *titulo, const char *artista, int duracao) {
    t_musica *m = malloc(sizeof(t_musica));
    if (m == NULL) return NULL;

    m->id = id;
    m->duracao = duracao;
    strncpy(m->titulo, titulo, sizeof(m->titulo) - 1);
    m->titulo[sizeof(m->titulo) - 1] = '\0';
    strncpy(m->artista, artista, sizeof(m->artista) - 1);
    m->artista[sizeof(m->artista) - 1] = '\0';

    return m;
}

void musica_destruir(void *carga) {
    free(carga);
}