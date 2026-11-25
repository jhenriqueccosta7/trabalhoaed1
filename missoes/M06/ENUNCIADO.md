# MiniPlayer — M06: TAD Pilha por composição

ICC002 — 25/11/2026 — duplas — 100 minutos

## Situação-problema
O histórico de reprodução precisa recuperar primeiro a música mais recentemente registrada. Implemente uma pilha genérica LIFO reutilizando a lista das M03–M05. A integração efetiva com o histórico fica para a próxima missão.

## Interface e contratos
Preserve o `include/pilha.h` do kit inicial:

```c
typedef struct pilha t_pilha;
t_pilha *pilha_criar(void);
int pilha_empilhar(t_pilha *pilha, void *carga);
void *pilha_desempilhar(t_pilha *pilha);
void *pilha_topo(const t_pilha *pilha);
void pilha_destruir(t_pilha *pilha, void (*destruir_carga)(void *));
```

| Operação | Delegação | Contrato |
|---|---|---|
| criar | lse_criar | Aloca a pilha e sua lista vazia; retorna NULL em falha e libera qualquer alocação parcial. |
| empilhar | lse_inserir_inicio | Retorna 1 em sucesso, 0 em falha ou pilha NULL; não altera a pilha em falha. |
| desempilhar | lse_remover_inicio | Retorna a carga do topo sem liberá-la; NULL para pilha vazia ou inválida. |
| topo | lse_primeiro | Consulta sem remover; NULL para pilha vazia ou inválida. |
| destruir | lse_destruir | Repassa o callback opcional e libera também o objeto pilha; NULL é operação sem efeito. |

Use cargas não nulas nesta missão: NULL como carga é ambíguo com o retorno de pilha vazia. Não é necessário alterar o contrato da lista nem acrescentar uma validação de carga à pilha. A pilha não copia cargas. No futuro histórico, as músicas pertencerão à biblioteca; use destruição com callback NULL para referências emprestadas. Se a pilha possuir cargas alocadas, o callback pode liberá-las, desde que cada carga tenha propriedade exclusiva.

## Atividades
1. Desenhe o estado da pilha após empilhar A, B, C, consultar topo, desempilhar e empilhar D. Indique a ordem de saída restante.
2. Examine os stubs de `src/pilha.c`. Preserve `struct pilha { t_lse *lista; };` privada. Inclua `<stdlib.h>` para alocação e liberação.
3. Implemente criação e destruição, tratando falhas parciais. Implemente as demais operações delegando à interface pública da lista. Não acesse `t_elem` ou campos de `t_lse`, não duplique encadeamento e não inclua `lse.c`.
4. Execute os testes da M06 e das missões anteriores. Acrescente um teste próprio de intercalação de operações.
5. Atualize as seções M06 em `docs/arquitetura.md`, `contratos.md`, `diario-de-bordo.md` e `relatorio-testes.md`. Registre a relação de composição, propriedade das cargas e complexidade.

## Reflexão
- Por que inserir e remover no início permite O(1) por operação? Qual seria o custo de remover no fim de uma LSE?
- Por que consultar o topo não pode remover a carga?
- Por que destruir a pilha do histórico não deve liberar as músicas da biblioteca?
- Qual a diferença entre composição e duplicação da implementação?

## Aceitação
Pilha opaca e genérica; operações LIFO corretas; topo não destrutivo; instâncias independentes; tratamento de vazio e NULL; destruição com e sem callback; testes anteriores preservados. Empilhar, desempilhar e topo devem ser O(1), assumindo as operações correspondentes da LSE O(1); destruição O(n), desconsiderando custo do callback. A inspeção do código é necessária para verificar composição e complexidade.

## Entrega
Commit da M06 com código, teste próprio e documentação cumulativa. Não modifique as asserções fornecidas para mascarar falhas. A integração do histórico não é exigida nesta missão.
