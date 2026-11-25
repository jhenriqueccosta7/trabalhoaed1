#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

static void lifo(void) {
    int a=1,b=2,c=3;
    t_pilha *p=pilha_criar(); assert(p);
    assert(pilha_topo(p)==NULL);
    assert(pilha_desempilhar(p)==NULL);
    assert(pilha_empilhar(p,&a)==1);
    assert(pilha_empilhar(p,&b)==1);
    assert(pilha_topo(p)==&b);
    assert(pilha_topo(p)==&b);
    assert(pilha_desempilhar(p)==&b);
    assert(pilha_empilhar(p,&c)==1);
    assert(pilha_desempilhar(p)==&c);
    assert(pilha_desempilhar(p)==&a);
    assert(pilha_desempilhar(p)==NULL);
    assert(pilha_empilhar(p,&a)==1);
    assert(pilha_topo(p)==&a);
    pilha_destruir(p,NULL);
}
static void independentes(void) {
    struct { int id; char titulo[16]; } a={1,"A"}, b={2,"B"};
    t_pilha *p=pilha_criar(), *q=pilha_criar(); assert(p && q);
    assert(pilha_empilhar(p,&a)==1);
    assert(pilha_empilhar(q,&b)==1);
    assert(pilha_desempilhar(p)==&a);
    assert(pilha_topo(q)==&b);
    pilha_destruir(p,NULL); pilha_destruir(q,NULL);
}
static int liberadas;
static void liberar(void *c) { assert(c); ++liberadas; free(c); }
static void propriedade(void) {
    t_pilha *p=pilha_criar(); assert(p);
    int *a=malloc(sizeof *a), *b=malloc(sizeof *b), *c=malloc(sizeof *c);
    assert(a && b && c); *a=1; *b=2; *c=3;
    assert(pilha_empilhar(p,a)==1);
    assert(pilha_empilhar(p,b)==1);
    assert(pilha_empilhar(p,c)==1);
    assert(pilha_desempilhar(p)==c); assert(*c==3); free(c);
    liberadas=0; pilha_destruir(p,liberar); assert(liberadas==2);
    p=pilha_criar(); assert(p); pilha_destruir(p,liberar); assert(liberadas==2);
}
static void invalidos(void) {
    int x=1;
    assert(pilha_empilhar(NULL,&x)==0);
    assert(pilha_topo(NULL)==NULL);
    assert(pilha_desempilhar(NULL)==NULL);
    pilha_destruir(NULL,liberar);
}
int main(void) {
    lifo(); independentes(); propriedade(); invalidos();
    puts("M06: todos os testes passaram."); return 0;
}
