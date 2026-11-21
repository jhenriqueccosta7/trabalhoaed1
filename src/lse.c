#include "lse.h"
#include <stddef.h>
#include "stdlib.h"
#include "assert.h"
#include "stdbool.h"

/* Representacao interna: NAO mover para lse.h. */
typedef struct elem {
    void *carga;
    struct elem *prox;
} t_elem;
struct lse {
    t_elem *inicio;
    int tamanho;
    /* M08 (otimizacao opcional): t_elem *fim; */
};
/* Stubs intencionalmente incompletos; testes devem falhar ate implementacao. */
t_lse *lse_criar(void) {
    t_lse *nova = malloc(sizeof(struct lse));
    if (nova == NULL) return NULL; 

    nova->inicio = NULL;
    nova->tamanho = 0;
        return nova;
    } /* TODO M02/M03 */

int lse_tamanho(const t_lse *lista) {
    assert(lista!=NULL);
    return lista->tamanho;
} /* TODO M03 */

void *lse_primeiro(const t_lse *lista) {
    if (lista == NULL || lista->inicio == NULL) {
        return NULL;
    }
    return lista->inicio->carga;
} /* TODO M03 */

static t_elem* criar_elem_lse(void* carga) {
    t_elem* novo = malloc(sizeof(t_elem));
    if (novo == NULL) return NULL;

    novo->carga = carga;
    novo->prox = NULL;
    return novo;
}

int lse_inserir_inicio(t_lse *lista, void *carga) { 
    if (lista == NULL) return 0;

    t_elem* novo = criar_elem_lse(carga);
    if (novo == NULL) {
        return 0;
    }
    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        novo->prox = lista->inicio;
        lista->inicio = novo;
    }
    lista->tamanho++;
    return 1;
}
/* TODO M04 */
int lse_inserir_fim(t_lse *lista, void *carga) {
    if (lista == NULL) return 0;

    t_elem *novo = criar_elem_lse(carga);
    if (novo == NULL) return 0;

    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        t_elem *cam = lista->inicio;
        while (cam->prox != NULL) {
            cam = cam->prox;
        }
        cam->prox = novo;
    }
    lista->tamanho++;
    return 1;
}

void *lse_remover_inicio(t_lse *lista) {
    if (lista == NULL || lista->tamanho == 0) {
        return NULL;
    }

    t_elem *prim = lista->inicio;
    lista->inicio = prim->prox;

    void *carga = prim->carga;
    lista->tamanho--;
    free(prim);
    return carga;
}

void *lse_buscar(const t_lse *lista, const void *chave, t_lse_comparador comparar) {
    if (lista == NULL || comparar == NULL) return NULL;
    t_elem *cam = lista->inicio;
    while (cam != NULL) {
        if (comparar(cam->carga, chave) == 0) {
            return cam->carga;
        } else {
            cam = cam->prox;
        }
    }
    return NULL;
}

void *lse_remover(t_lse *lista, const void *chave, t_lse_comparador comparar) {
    if (lista == NULL || comparar == NULL) return NULL;
    bool achou = false;
    t_elem *cam = lista->inicio;
    t_elem *ant = NULL;

    while (!achou && cam != NULL) {
        if (comparar(cam->carga, chave) == 0) {
            achou = true;
        } else {
            ant = cam;
            cam = cam->prox;
        }
    }
    if (!achou) {
        return NULL;
    } else {
        if (lista->inicio == cam) {
            lista->inicio = cam->prox;
        } else {
            ant->prox = cam->prox;
        }
        void *carga = cam->carga;
        lista->tamanho--;
        free(cam);
        return carga;
    }
}

int lse_inserir_ordenado(t_lse *lista, void *carga, t_lse_comparador comparar) {
    assert(lista != NULL);
    assert(comparar != NULL);

    t_elem *novo = criar_elem_lse(carga);
    if (novo == NULL) return 0;

    t_elem *ant = NULL;
    t_elem *cam = lista->inicio;

    while (cam != NULL && comparar(cam->carga, carga) < 0) {
        ant = cam;
        cam = cam->prox;
    }

    if (ant == NULL) {
        novo->prox = lista->inicio;
        lista->inicio = novo;
    } else {
        novo->prox = cam;
        ant->prox = novo;
    }

    lista->tamanho++;
    return 1;
}

void lse_percorrer(const t_lse *lista, t_visitar cb_visitar) {
    if (lista == NULL || cb_visitar == NULL) return;

    t_elem *cam = lista->inicio;
    while (cam != NULL) {
        cb_visitar(cam->carga);
        cam = cam->prox;
    }
}

void lse_destruir(t_lse *lista, t_lse_destruir_carga destruir_carga) {
    if (lista == NULL) return;

    t_elem *cam = lista->inicio;
    while (cam != NULL) {
        t_elem *prox = cam->prox;
        if (destruir_carga != NULL) {
            destruir_carga(cam->carga);
        }
        free(cam);
        cam = prox;
    }
    free(lista);
}