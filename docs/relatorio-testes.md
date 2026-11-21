# Relatório de testes
| ID | Missão | Função/contrato | Entrada/cenário | Resultado esperado | Resultado obtido | Status |
|---|---|---|---|---|---|---|
| T01 | M01 | musica_criar / musica_destruir | id 1, "Teste", "Artista", 180 | ponteiro não nulo e campos corretos | campos conferidos; `test_musica: OK` | passou |
| T02 | M02 | lse_criar / tamanho / primeiro / destruir | lista recém-criada | tamanho 0, primeiro `NULL`, destruição sem erro | tamanho 0, primeiro `NULL` | passou |
| T03 | M03 | lse_percorrer | lista vazia | nenhuma visita | 0 visitas | passou |
| T04 | M03 | lse_inserir_inicio / primeiro / percorrer | inserção de 10, 20, 30 na cabeça | ordem LIFO (30, 20, 10), tamanho 3 | ordem 30, 20, 10 confirmada | passou |
| T05 | M03 | lse_inserir_inicio | carga de outro tipo (struct com id e nome) | tamanho 2, primeiro correto | conforme esperado | passou |
| T06 | M03 | lse_destruir | 2 inteiros no heap, com callback | callback chamado 2 vezes | `liberacoes == 2` | passou |
| T07 | M03 | lse_inserir_inicio | `lista == NULL` | retorno `0`, sem acesso inválido | retorno `0` | passou |
| T08 | M04 | lse_inserir_fim / buscar | inserção de 10, 20, 30; busca do primeiro, meio, último e de 99 | ordem de inserção preservada; ponteiros corretos; `NULL` para 99; lista inalterada | conforme esperado | passou |
| T09 | M04 | lse_remover | lista 10, 20, 30, 40; remoção do meio (20), cauda (40) e cabeça (10) | religamento correto e tamanho decrementado | sobra apenas 30, tamanho 1 | passou |
| T10 | M04 | lse_remover | chave ausente (99) em lista 5, 5, 8 | retorna `NULL`, lista inalterada | conforme esperado | passou |
| T11 | M04 | lse_remover_inicio | lista vazia; depois 5, 5, 8 removidos um a um | `NULL` na vazia; cargas na ordem `&a`, `&b`, `&c`; `NULL` ao esvaziar | conforme esperado | passou |
| T12 | M04 | lse_buscar / remover | registro `musica_t` com comparador por `id` | localiza e remove `id = 22` | `&b` retornado; resta 1 elemento | passou |
| T13 | M04 | todas as funções da M04 | `lista == NULL` e `comparar == NULL` | retorno `0` ou `NULL`, sem falha | retornos defensivos confirmados | passou |
 
## Casos-limite
- **Estrutura vazia:** `lse_tamanho` retorna `0`; `lse_primeiro`, `lse_remover_inicio`, `lse_buscar` e `lse_remover` retornam `NULL`; `lse_percorrer` não chama o callback.
- **Estrutura com um elemento:** remover pela cabeça ou por chave deixa `inicio == NULL` e `tamanho == 0`.
- **Chave inexistente:** `lse_buscar` e `lse_remover` percorrem até `NULL` e retornam `NULL`, sem alterar a lista nem o `tamanho`.
- **Chave duplicada:** `lse_remover` para no primeiro nó correspondente, então remove apenas a primeira ocorrência.
- **Falha de alocação (discutir estratégia de simulação):** `lse_criar`, `lse_inserir_inicio` e `lse_inserir_fim` verificam o retorno de `malloc` e retornam `NULL`/`0` sem alterar a lista. Uma forma de simular a falha seria um *wrapper* de `malloc` que falha após N chamadas, ou limitar a memória do processo com `ulimit -v`.
## Memória
- **Compilação e execução:** `make test_missoes` (gcc com `-std=c11 -Wall -Wextra -Wpedantic -g -Iinclude`).
- **Verificação de memória:** os testes foram compilados com AddressSanitizer, por exemplo:
  `gcc -std=c11 -g -fsanitize=address,undefined -Iinclude missoes/M04/test_m04.c src/lse.c -o build/asan_m04 && ./build/asan_m04`
  Resultado: nenhum acesso inválido e nenhum vazamento em M02, M03, M04 e `test_musica`.
- **Problemas corrigidos:**
  1. O `test_m03.c` vinha com as funções de teste contendo apenas `assert(1==0)` (falha proposital até a implementação dos testes); escrevemos os casos de teste T03 a T07.
  2. Funções da M04 abortavam com `assert(lista != NULL)` quando recebiam lista nula; trocamos por retorno defensivo (`0`/`NULL`), conforme o contrato da missão.
  3. `lse_buscar` e `lse_remover` não verificavam `comparar == NULL`, o que causaria falha de segmentação; incluímos a verificação.
 