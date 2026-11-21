# Arquitetura
## Componentes
| Módulo | Responsabilidade | Dependências |
|---|---|---|
| musica | Dados da música | biblioteca padrão |
| lse | Nós genéricos e encadeamento | biblioteca padrão |
| pilha | Política LIFO | lse |
| fila | Política FIFO | lse |
| miniplayer | Simulação e integração | demais TADs |
 
## Diagrama de dependências
MiniPlayer → (LSE, Pilha, Fila) → LSE → t_elem interno.
 
## Decisões técnicas
- **Por que `void *`?** Permite que a LSE armazene qualquer tipo de dado (`t_musica`, `int`, etc.) sem duplicar o código de encadeamento e sem que `lse.c` precise incluir `musica.h`. A lista não conhece o tipo da carga; quem conhece é o chamador, que fornece os callbacks de comparação, visita e destruição.
- **Quem é dono das cargas?** A LSE é dona apenas dos seus nós (`t_elem`) e do descritor (`t_lse`). A carga pertence ao módulo que a criou (ex.: a biblioteca do MiniPlayer). Em `lse_destruir`, a lista aceita um callback opcional (`t_lse_destruir_carga`); se for `NULL`, libera apenas os nós.
- **Como tratar referências compartilhadas?** Uma mesma música pode estar ao mesmo tempo na biblioteca, na fila de reprodução e no histórico. Apenas a estrutura dona da carga passa um callback destrutor em `lse_destruir`; as demais passam `NULL`, liberando só os próprios nós. Isso evita *double free*.
- **Quando adicionar ponteiro `fim`?** Conforme o escopo da M04, `lse_inserir_fim` percorre a cadeia em O(n). Um ponteiro `fim` exigiria atualizá-lo em todas as remoções (inclusive da cauda), o que aumenta o risco de erro neste momento. Ele fica reservado para a otimização opcional da M08.
---
 
## Missão M02 — Encapsulamento e tipos opacos
- **Ocultamento de representação (`t_lse` e `t_elem`):** `include/lse.h` declara apenas `typedef struct lse t_lse;`. As definições de `struct lse` e de `t_elem` ficam privadas em `src/lse.c`. Assim, nenhum módulo externo consegue manipular `prox` ou `tamanho` diretamente e quebrar os invariantes da lista.
## Missão M03 — Construção, encadeamento e percurso
- **Desenho de encadeamento (inserção no início — LIFO na cabeça):**
  1. Lista vazia: `inicio -> NULL` (`tamanho == 0`).
  2. `lse_inserir_inicio(l, &A)`: `novo->prox = NULL`, `inicio -> [A|NULL]` (`tamanho == 1`).
  3. `lse_inserir_inicio(l, &B)`: `novo->prox = inicio` (`[A]`), `inicio -> [B|*] -> [A|NULL]` (`tamanho == 2`).
- **Percurso sem expor nós (`lse_percorrer`):** a lista itera internamente com `t_elem *cam` e chama `cb_visitar(cam->carga)`, entregando ao chamador apenas a carga genérica, nunca o nó.
## Missão M04 — Operações por chave e remoção
- **Inserção no fim (`lse_inserir_fim`):** sem ponteiro `fim`, percorre até o nó com `prox == NULL` e liga o novo nó ali. Em lista vazia, o novo nó passa a ser o `inicio`.
- **Desenho de remoção (`lse_remover`):** a busca usa dois ponteiros, `ant` (anterior) e `cam` (atual).
  - *Lista vazia:* `cam` começa em `NULL`, o laço não executa e a função retorna `NULL` sem alterar nada.
  - *Elemento único (`inicio -> [A|NULL]`):* `lista->inicio = cam->prox` (`NULL`), libera o nó de `[A]`, `tamanho = 0`.
  - *Cabeça em lista de 3 nós (`[A] -> [B] -> [C]`):* `lista->inicio = cam->prox` (`[B]`), libera o nó de `[A]`, `tamanho = 2`.
  - *Meio (`[B]`):* `ant->prox = cam->prox` (`[A] -> [C]`), libera o nó de `[B]`, `tamanho = 2`.
  - *Cauda (`[C]`):* `ant->prox = cam->prox` (`NULL`), libera o nó de `[C]`, `tamanho = 2`.
  - Em todos os casos, apenas o **nó** é liberado; a carga é devolvida ao chamador.
- **Ambiguidade do retorno `NULL`:** a M03 permite inserir carga `NULL`, e a M04 usa `NULL` para sinalizar "não encontrado", "lista vazia" ou "argumento inválido". Assim, não é possível distinguir uma falha de um nó encontrado cuja carga era `NULL`. Por isso, os testes de busca e remoção não usam cargas `NULL`. Uma API futura poderia retornar um código de status (`int`: `1` sucesso, `0` falha) e devolver a carga por um parâmetro de saída (`void **carga_saida`).
 
