# Contratos dos TADs
Para **cada função**, preencher:
- Nome e assinatura:
- Pré-condições:
- Pós-condições:
- Valor de retorno em sucesso/falha:
- Quem aloca/libera a memória:
- Invariantes preservadas:
- Testes associados:
## Invariantes da LSE
- `tamanho >= 0`.
- `tamanho == 0` se e somente se `inicio == NULL`.
- Há exatamente `tamanho` nós alcançáveis a partir de `inicio`.
- O último nó aponta para `NULL` e não há ciclos.
## Convenção sobre argumentos inválidos
Usamos dois estilos, conforme o contrato de cada missão:
- **Pré-condição verificada por `assert`:** a chamada com argumento inválido é considerada erro de programação e o programa aborta (ex.: `lse_tamanho`, `lse_percorrer`).
- **Tratamento defensivo:** a função aceita o argumento inválido e retorna um valor de erro (`0` ou `NULL`) sem acessar memória indevida (ex.: `lse_inserir_inicio` e todas as funções da M04).
Nas funções defensivas, a seção "Pré-condições" indica "Nenhuma", e o comportamento para entradas inválidas aparece em "Valor de retorno".
 
---
 
## Contratos — Missão M01
### `t_musica *musica_criar(int id, const char *titulo, const char *artista, int duracao)`
- **Pré-condições:** `titulo != NULL` e `artista != NULL` (strings terminadas em `'\0'`).
- **Pós-condições:** Aloca um `t_musica` com os campos preenchidos. `titulo` e `artista` são copiados com `strncpy` e truncados em 79 caracteres, sempre terminando em `'\0'`.
- **Valor de retorno:** Ponteiro válido em sucesso, ou `NULL` em falha de alocação.
- **Memória:** Aloca um `t_musica`; o chamador libera com `musica_destruir`.
- **Testes:** `tests/test_musica.c` (`test_musica`).
### `void musica_destruir(void *carga)`
- **Pré-condições:** `carga` foi criada por `musica_criar` (ou é `NULL`).
- **Pós-condições:** A memória da música é liberada.
- **Valor de retorno:** `void`.
- **Memória:** Libera o `t_musica`. A assinatura com `void *` permite usá-la diretamente como callback de `lse_destruir`.
- **Testes:** `test_musica`.
---
 
## Contratos — Missão M02
### `t_lse *lse_criar(void)`
- **Pré-condições:** Nenhuma.
- **Pós-condições:** Aloca um descritor `t_lse` vazio com `inicio == NULL` e `tamanho == 0`.
- **Valor de retorno:** Ponteiro válido para `t_lse` em sucesso, ou `NULL` em falha de alocação.
- **Memória:** Aloca `struct lse`; liberada por `lse_destruir`.
- **Invariantes:** `tamanho == 0` e `inicio == NULL`.
- **Testes:** `test_m02`.
### `int lse_tamanho(const t_lse *lista)`
- **Pré-condições:** `lista != NULL` (verificada por `assert`; contrato adotado pela dupla, conforme permitido no enunciado da M03).
- **Pós-condições:** Lista inalterada.
- **Valor de retorno:** Número atual de nós (`>= 0`).
- **Memória:** Não aloca nem libera memória.
- **Invariantes:** Todas preservadas.
- **Testes:** `test_m02`, `test_m03`, `test_m04`.
### `void *lse_primeiro(const t_lse *lista)`
- **Pré-condições:** Nenhuma (aceita `lista == NULL` ou lista vazia).
- **Pós-condições:** Lista inalterada.
- **Valor de retorno:** Carga do primeiro nó, ou `NULL` se `lista == NULL` ou vazia.
- **Memória:** Não aloca nem libera memória.
- **Invariantes:** Todas preservadas.
- **Testes:** `test_m02`, `test_m03`.
### `void lse_destruir(t_lse *lista, t_lse_destruir_carga destruir_carga)`
- **Pré-condições:** Nenhuma (se `lista == NULL`, não faz nada).
- **Pós-condições:** Todos os nós e o descritor são liberados. Se `destruir_carga != NULL`, o callback é chamado uma vez para cada carga antes de liberar o nó correspondente.
- **Valor de retorno:** `void`.
- **Memória:** Libera todos os `t_elem` e `struct lse`; as cargas só são liberadas se `destruir_carga != NULL`. A função não presume que as cargas foram alocadas dinamicamente.
- **Invariantes:** Estrutura completamente desalocada; o ponteiro `lista` não deve mais ser usado.
- **Testes:** `test_m02`, `test_m03` (`teste_destruicao_com_callback`).
---
 
## Contratos — Missão M03
### `int lse_inserir_inicio(t_lse *lista, void *carga)`
- **Pré-condições:** Nenhuma. `carga == NULL` é aceita (decisão da dupla: um nó com carga nula continua sendo um nó da estrutura).
- **Pós-condições:** Em sucesso, um novo nó passa a ser o `inicio`, apontando para o antigo primeiro nó, e `tamanho` aumenta em 1. Em falha, a lista não é alterada.
- **Valor de retorno:** `1` em sucesso; `0` se `lista == NULL` ou se a alocação falhar.
- **Memória:** Aloca um `t_elem`.
- **Invariantes:** Preservadas; após sucesso, `inicio != NULL` e `tamanho > 0`.
- **Testes:** `test_m03` (`teste_encadeamento_e_percurso`, `teste_carga_generica`, chamada com `NULL` em `main`).
### `void lse_percorrer(const t_lse *lista, t_visitar cb_visitar)`
- **Pré-condições:** `lista != NULL` (verificada por `assert`). Se `cb_visitar == NULL`, retorna sem fazer nada.
- **Pós-condições:** Chama `cb_visitar(carga)` uma vez para cada nó, do primeiro ao último, sem modificar a lista. Em lista vazia, o callback não é chamado.
- **Valor de retorno:** `void`.
- **Memória:** Não aloca nem libera memória.
- **Invariantes:** Todas preservadas.
- **Testes:** `test_m03` (`teste_lista_vazia`, `teste_encadeamento_e_percurso`); também usada como verificador em todos os cenários do `test_m04`.
- **Observação:** o enunciado da M03 sugere que `lse_percorrer(NULL, ...)` não faça nada. Mantivemos o `assert` visto em aula, tratando `lista != NULL` como pré-condição. Nenhum teste chama a função com `NULL`. Para seguir o enunciado, bastaria trocar o `assert` por `if (lista == NULL || cb_visitar == NULL) return;`.
---
 
## Contratos — Missão M04
### `int lse_inserir_fim(t_lse *lista, void *carga)`
- **Pré-condições:** Nenhuma (tratamento defensivo).
- **Pós-condições:** Em sucesso, o novo nó é ligado após o último nó (ou vira o `inicio`, se a lista estava vazia) e `tamanho` aumenta em 1. A ordem dos demais nós é preservada.
- **Valor de retorno:** `1` em sucesso; `0` se `lista == NULL` ou se a alocação falhar.
- **Memória:** Aloca um `t_elem`.
- **Complexidade:** O(n), pois não há ponteiro `fim`.
- **Invariantes:** O último nó aponta para `NULL`; `tamanho` igual ao número de nós.
- **Testes:** `test_m04` (`teste_inserir_fim_e_buscar`, `teste_argumentos_invalidos`).
### `void *lse_buscar(const t_lse *lista, const void *chave, t_lse_comparador comparar)`
- **Pré-condições:** Nenhuma (tratamento defensivo). O comparador não deve alterar a carga nem a chave.
- **Pós-condições:** Lista inalterada.
- **Valor de retorno:** A **primeira** carga para a qual `comparar(carga, chave) == 0`; `NULL` se não encontrar, se a lista estiver vazia, ou se `lista == NULL` ou `comparar == NULL`.
- **Memória:** Não aloca nem libera memória.
- **Invariantes:** Todas preservadas.
- **Testes:** `test_m04` (`teste_inserir_fim_e_buscar`, `teste_generico_musicas`, `teste_argumentos_invalidos`).
### `void *lse_remover(t_lse *lista, const void *chave, t_lse_comparador comparar)`
- **Pré-condições:** Nenhuma (tratamento defensivo). O comparador não deve alterar a carga nem a chave.
- **Pós-condições:** Remove apenas o **primeiro** nó correspondente, religando `inicio` (se for a cabeça) ou `ant->prox` (nos demais casos), e diminui `tamanho` em 1. Se não encontrar, a lista não é alterada.
- **Valor de retorno:** Carga do nó removido; `NULL` se não encontrar, ou se `lista == NULL` ou `comparar == NULL`.
- **Memória:** Libera **apenas o nó**; a responsabilidade pela carga volta ao chamador.
- **Invariantes:** Encadeamento contínuo, sem nós perdidos; se `tamanho == 0`, `inicio == NULL`.
- **Testes:** `test_m04` (`teste_remover_cabeca_meio_cauda`, `teste_remover_inicio`, `teste_generico_musicas`, `teste_argumentos_invalidos`).
### `void *lse_remover_inicio(t_lse *lista)`
- **Pré-condições:** Nenhuma (tratamento defensivo).
- **Pós-condições:** Se a lista não estiver vazia, remove o primeiro nó, faz `inicio = antigo_inicio->prox` e diminui `tamanho` em 1.
- **Valor de retorno:** Carga do antigo primeiro nó; `NULL` se a lista estiver vazia ou se `lista == NULL`.
- **Memória:** Libera **apenas o nó**.
- **Invariantes:** Se `tamanho == 0`, `inicio == NULL`.
- **Testes:** `test_m04` (`teste_remover_inicio`, `teste_argumentos_invalidos`).