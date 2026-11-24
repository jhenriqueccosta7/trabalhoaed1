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


static const void *esperados[8];
static int visitas;
static void verificar(void *carga) {
    assert(visitas < 8);
    assert(carga == esperados[visitas]);
    visitas++;
}
static void conferir(const t_lse *l, int n) {
    assert(lse_tamanho(l) == n);
    visitas = 0;
    lse_percorrer(l, verificar);
    assert(visitas == n);
}

static void teste_vazia_inicio_meio_fim(void) {
    int v30 = 30, v10 = 10, v20 = 20, v40 = 40;
    t_lse *l = lse_criar(); assert(l);

    assert(lse_inserir_ordenado(l, &v30, comparar_int) == 1);   /* vazia  */
    esperados[0] = &v30;
    conferir(l, 1);

    assert(lse_inserir_ordenado(l, &v10, comparar_int) == 1);   /* início */
    esperados[0] = &v10; esperados[1] = &v30;
    conferir(l, 2);
    assert(lse_primeiro(l) == &v10);

    assert(lse_inserir_ordenado(l, &v20, comparar_int) == 1);   /* meio   */
    esperados[0] = &v10; esperados[1] = &v20; esperados[2] = &v30;
    conferir(l, 3);

    assert(lse_inserir_ordenado(l, &v40, comparar_int) == 1);   /* fim    */
    esperados[3] = &v40;
    conferir(l, 4);

    lse_destruir(l, NULL);
}

static void teste_duplicados_estaveis(void) {
    musica_t a={10,"primeira"}, b={10,"segunda"}, c={5,"menor"}, d={10,"terceira"};
    t_lse *l = lse_criar(); assert(l);

    assert(lse_inserir_ordenado(l, &a, comparar_musica) == 1);
    assert(lse_inserir_ordenado(l, &b, comparar_musica) == 1);
    assert(lse_inserir_ordenado(l, &c, comparar_musica) == 1);
    assert(lse_inserir_ordenado(l, &d, comparar_musica) == 1);

    /* id 5 primeiro; os três de id 10 na ordem de chegada (estabilidade). */
    esperados[0] = &c; esperados[1] = &a; esperados[2] = &b; esperados[3] = &d;
    conferir(l, 4);

    lse_destruir(l, NULL);
}

static void teste_invalido_e_reinsercao(void) {
    int v10 = 10, v20 = 20, v30 = 30;
    t_lse *l = lse_criar(); assert(l);

    /* Argumentos inválidos: retorna 0 e a lista não muda. */
    assert(lse_inserir_ordenado(NULL, &v10, comparar_int) == 0);
    assert(lse_inserir_ordenado(l, &v10, NULL) == 0);
    conferir(l, 0);

    assert(lse_inserir_ordenado(l, &v10, comparar_int) == 1);
    assert(lse_inserir_ordenado(l, &v20, comparar_int) == 1);
    assert(lse_inserir_ordenado(l, &v30, comparar_int) == 1);
    assert(lse_inserir_ordenado(l, &v10, NULL) == 0);
    esperados[0] = &v10; esperados[1] = &v20; esperados[2] = &v30;
    conferir(l, 3);

    /* Remoção seguida de reinserção ordenada: volta para o meio. */
    assert(lse_remover(l, &v20, comparar_int) == &v20);
    esperados[0] = &v10; esperados[1] = &v30;
    conferir(l, 2);
    assert(lse_inserir_ordenado(l, &v20, comparar_int) == 1);
    esperados[0] = &v10; esperados[1] = &v20; esperados[2] = &v30;
    conferir(l, 3);

    lse_destruir(l, NULL);
}

int main(void) {
    teste_vazia_inicio_meio_fim();
    teste_duplicados_estaveis();
    teste_invalido_e_reinsercao();
    puts("M05: todos os testes passaram.");
    return 0;
}