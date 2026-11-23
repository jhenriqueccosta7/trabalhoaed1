/* Testes incrementais M05 — não acessar campos privados de t_lse/t_elem. */
#include <assert.h>
#include <stdio.h>
#include "lse.h"

static int comparar_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

typedef struct { int id; const char *titulo; } musica_t;
static int comparar_musica(const void *a, const void *b) {
    int x = ((const musica_t *)a)->id, y = ((const musica_t *)b)->id;
    return (x > y) - (x < y);
}


static void teste_vazia_inicio_meio_fim(void) {
    t_lse *l = lse_criar(); assert(l);
    assert(1==0);
    lse_destruir(l,NULL);
}

static void teste_duplicados_estaveis(void) {
    musica_t a={10,"primeira"}, b={10,"segunda"}, c={5,"menor"}, d={10,"terceira"};
    t_lse *l=lse_criar(); assert(l);
 
    lse_destruir(l,NULL);
    assert(1==0);
}

static void teste_invalido_e_reinsercao(void) {
    assert(1==0);
}

int main(void) {
    teste_vazia_inicio_meio_fim();
    teste_duplicados_estaveis();
    teste_invalido_e_reinsercao();
    puts("M05: todos os testes passaram.");
    return 0;
}
