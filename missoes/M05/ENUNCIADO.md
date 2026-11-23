# MiniPlayer — Missão 05: Inserção ordenada e testes

**ICC002 — Algoritmos e Estruturas de Dados I**  
**Data:** segunda-feira, 23/11/2026  
**Modalidade:** duplas | **Duração:** 2 horas-aula

## Situação-problema

O MiniPlayer já permite inserir, percorrer, buscar e remover elementos de uma lista simplesmente encadeada. Agora a biblioteca precisa receber novas músicas em ordem, sem ordenar toda a coleção depois de cada inserção.

A missão consiste em implementar **inserção ordenada genérica**, preservando o encapsulamento de `t_elem` e a representação opaca de `t_lse`.

## Objetivos

1. Implementar `lse_inserir_ordenado` utilizando encadeamento de ponteiros.
2. Utilizar `t_lse_comparador` para decidir a posição de inserção.
3. Tratar lista vazia, início, meio, fim e valores iguais.
4. Preservar os invariantes de `inicio` e `tamanho`.
5. Executar testes da M05 e regressão das missões anteriores.

## Contrato da operação

A assinatura já está prevista no `include/lse.h` do kit inicial:

```c
int lse_inserir_ordenado(t_lse *lista, void *carga,
                         t_comparador comparar);
```

**Pré-condições:** `lista != NULL` e `comparar != NULL`. A lista deve estar previamente ordenada pelo **mesmo critério de comparação**. Não é responsabilidade da operação corrigir uma lista previamente desordenada.

**Resultado:** retorna `1` quando a inserção é concluída e `0` para argumentos inválidos ou falha de alocação. Em caso de falha, a lista não deve mudar.

**Comparação:** nesta operação, a função é chamada como `comparar(carga_existente, carga_nova)`; ambos os argumentos apontam para **cargas** (diferentemente da busca por chave da M04). O resultado é negativo, zero ou positivo conforme a primeira carga seja menor, igual ou maior que a segunda.

**Empates:** insira o novo elemento **depois** dos elementos já existentes com a mesma chave, mantendo a ordem de chegada (*estabilidade*).

**Propriedade de memória:** a lista aloca e libera nós, mas não copia nem assume automaticamente a propriedade das cargas. A inserção não deve alocar uma cópia de `t_musica`.

**Invariantes:** o campo `tamanho` aumenta exatamente uma unidade após sucesso; `inicio` aponta para o primeiro nó ou `NULL` quando vazia; nenhum nó pode ficar inacessível ou formar ciclo.

## Atividades

1. Desenhe os ponteiros `anterior`, `atual` e `novo` para quatro situações: vazia, início, meio e fim.
2. Escreva pseudocódigo que avance enquanto `comparar(atual->carga, carga_nova) <= 0`.
3. Implemente a operação **apenas no `src/lse.c` existente**. Não exponha `t_elem` no cabeçalho.
4. Execute `make test_m05` e também `make test_m03` e `make test_m04` (se instaladas).
5. Documente complexidade de tempo e espaço e as decisões sobre empates.

## Questões para reflexão

- Por que a inserção no início pode ser O(1), mas a inserção ordenada pode custar O(n)?
- Qual é o risco de usar critérios diferentes para duas inserções na mesma lista?
- Como o comparador permite ordenar músicas por título, duração ou identificador sem alterar `lse.c`?
- O que ocorre com a ordem relativa de elementos com chaves iguais?

## Critérios de aceitação

- [ ] A lista permanece ordenada após cada inserção.
- [ ] Inserção em lista vazia, início, meio e fim funciona.
- [ ] Elementos de mesma chave preservam a ordem de chegada.
- [ ] `tamanho` permanece correto.
- [ ] `t_elem` continua privado de `lse.c`.
- [ ] Testes com inteiros e registros de música passam.
- [ ] Testes anteriores continuam funcionando.
- [ ] Documentação cumulativa atualizada.


**Próxima missão:** M06 — Pilha como especialização da lista, em 25/11/2026.
