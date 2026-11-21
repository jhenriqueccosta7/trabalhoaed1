/* Testes aditivos da M03. Executar a partir da raiz com o Makefile incremental. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "lse.h"

static int ordem_esperada[4];
static int idx_visita = 0;
static int liberacoes = 0;

static void verificar_ordem(void *carga) {
    assert(idx_visita < 4);
    assert(*(int *)carga == ordem_esperada[idx_visita]);
    idx_visita++;
}

static void destruir_int(void *carga) {
    free(carga);
    liberacoes++;
}

static void teste_lista_vazia(void) {
    t_lse *l = lse_criar();
    assert(l != NULL);
    assert(lse_tamanho(l) == 0);
    assert(lse_primeiro(l) == NULL);

    idx_visita = 0;
    lse_percorrer(l, verificar_ordem);
    assert(idx_visita == 0);

    lse_destruir(l, NULL);
}

static void teste_encadeamento_e_percurso(void) {
    int a = 10, b = 20, c = 30;
    t_lse *l = lse_criar();
    assert(l != NULL);

    assert(lse_inserir_inicio(l, &a) == 1);
    assert(lse_tamanho(l) == 1);
    assert(lse_primeiro(l) == &a);

    assert(lse_inserir_inicio(l, &b) == 1);
    assert(lse_inserir_inicio(l, &c) == 1);
    assert(lse_tamanho(l) == 3);
    assert(lse_primeiro(l) == &c);

    /* Insercao no inicio produz ordem LIFO: 30, 20, 10 */
    ordem_esperada[0] = 30;
    ordem_esperada[1] = 20;
    ordem_esperada[2] = 10;
    idx_visita = 0;
    lse_percorrer(l, verificar_ordem);
    assert(idx_visita == 3);

    lse_destruir(l, NULL);
}

static void teste_carga_generica(void) {
    typedef struct { int id; char nome[16]; } item_t;
    item_t x = {1, "Faixa 1"}, y = {2, "Faixa 2"};

    t_lse *l = lse_criar();
    assert(l != NULL);
    assert(lse_inserir_inicio(l, &x) == 1);
    assert(lse_inserir_inicio(l, &y) == 1);
    assert(lse_tamanho(l) == 2);
    assert(lse_primeiro(l) == &y);

    lse_destruir(l, NULL);
}

static void teste_destruicao_com_callback(void) {
    t_lse *l = lse_criar();
    assert(l != NULL);

    int *p1 = malloc(sizeof(int));
    int *p2 = malloc(sizeof(int));
    assert(p1 != NULL && p2 != NULL);
    *p1 = 100;
    *p2 = 200;

    assert(lse_inserir_inicio(l, p1) == 1);
    assert(lse_inserir_inicio(l, p2) == 1);

    liberacoes = 0;
    lse_destruir(l, destruir_int);
    assert(liberacoes == 2);
}

int main(void) {
    teste_lista_vazia();
    teste_encadeamento_e_percurso();
    teste_carga_generica();
    teste_destruicao_com_callback();
    assert(lse_inserir_inicio(NULL, NULL) == 0);
    puts("M03: todos os testes passaram.");
    return 0;
}