# Diário de bordo
## Missão M01 — 09/11
- **Integrantes e papéis:** José Henrique da Cruz Costa (Implementação e organização do repositório) e André Luiz de Souza Justino (Testes e revisão de contratos).
- **Objetivo:** Implementar o TAD `t_musica` (criação e destruição de uma música).
- **O que implementamos:** `musica_criar` e `musica_destruir` em `src/musica.c`.
- **Decisão técnica e justificativa:** Cópia de `titulo` e `artista` com `strncpy` limitado ao tamanho do vetor e terminação `'\0'` forçada, evitando estouro de buffer com textos longos. `musica_destruir` recebe `void *` para poder ser usada como callback de `lse_destruir`.
- **Problema encontrado e solução:** Nenhum problema registrado até esse momento.
- **Evidências de testes:** `./build/test_musica` -> `test_musica: OK`.
- **Próxima ação:** Definir a interface e os contratos da lista (M02).
## Missão M02 — 11/11
- **Integrantes e papéis:** José Henrique (Documentação e validação) e André Luiz de Souza Justino (Implementação do tipo opaco).
- **Objetivo:** Definir o tipo opaco `t_lse` e implementar criação, consulta e destruição de lista vazia.
- **O que implementamos:** `lse_criar`, `lse_tamanho`, `lse_primeiro` e `lse_destruir` em `src/lse.c`, mantendo `t_elem` e `struct lse` privados.
- **Decisão técnica e justificativa:** Tipo opaco em `include/lse.h` para impedir que outros módulos dependam da estrutura interna da lista.
- **Problema encontrado e solução:** Nenhum conflito na instalação incremental via `missoes/M02/Makefile.incremental`.
- **Evidências de testes:** `./build/m02_incremental` -> `M02: teste de contrato passou.`
- **Próxima ação:** Implementar encadeamento de nós e percurso por callback (M03).
## Missão M03 — 16/11
- **Integrantes e papéis:** José Henrique da Cruz Costa (Implementação de encadeamento e percurso e relatório) e André Luiz de Souza Justino (Escrita dos testes unitários e relatório).
- **Objetivo:** Implementar inserção na cabeça, percurso via callback e destruição de nós com callback opcional para cargas.
- **O que implementamos:** `lse_inserir_inicio`, `lse_percorrer` e a versão completa de `lse_destruir`, além dos casos de teste em `missoes/M03/test_m03.c`.
- **Decisão técnica e justificativa:** Permitir `carga == NULL` na inserção (um nó continua sendo um nó) e delegar a liberação da carga ao callback opcional de `lse_destruir`. Em `lse_percorrer`, mantivemos `assert(lista != NULL)` como pré-condição, conforme visto em aula.
- **Problema encontrado e solução:** O `test_m03.c` fornecido pelo professor vinha com as funções de teste contendo apenas `assert(1==0)`, para garantir que os testes esperados falhassem até serem implementados (confirmado pelo professor em 20/11). Escrevemos os testes de lista vazia, inserção com ordem LIFO, carga genérica e destruição com callback, até todos passarem.
- **Evidências de testes:** `./build/test_m03_incremental` -> `M03: todos os testes passaram.`
- **Próxima ação:** Implementar inserção no fim, busca e remoção por chave (M04).
## Missão M04 — 18/11
- **Integrantes e papéis:** José Henrique da Cruz Costa (Depuração, testes incrementais e documentação) e André Luiz de Souza Justino (Implementação da busca/remoção e documentação).
- **Objetivo:** Implementar `lse_inserir_fim`, `lse_buscar`, `lse_remover` e `lse_remover_inicio` com comparador genérico.
- **O que implementamos:** As quatro operações em `src/lse.c` e os cenários de cabeça, meio, cauda, chave ausente, argumentos inválidos e registro do tipo `musica_t` em `missoes/M04/test_m04.c`.
- **Decisão técnica e justificativa:** Separar a liberação do nó (`free(cam)`) da propriedade da carga: a carga removida é devolvida ao chamador, que decide se deve liberá-la.
- **Problema encontrado e solução:** O teste `teste_argumentos_invalidos` falhava com `Assertion 'lista != NULL' failed` em `lse_inserir_fim`. O teste espera retorno `0`/`NULL` para `lista == NULL`, conforme o contrato do enunciado da M04, mas a função abortava no `assert`. Substituímos os `assert` das quatro funções da M04 por verificações com `if`, e acrescentamos a verificação de `comparar == NULL` em `lse_buscar` e `lse_remover` (sem ela, a chamada com comparador nulo causaria falha de segmentação). Os testes da M02 e M03 continuaram passando.
- **Evidências de testes:** `make test_missoes` -> M02, M03 e M04 aprovados. Execução com AddressSanitizer sem erros nem vazamentos.
- **Controle de versão e itens não marcados nos checklists:** Nas missões M01 a M03, não entendemos que os checklists pediam commits (ou cópias de segurança) antes de começar e ao concluir cada missão. Focamos diretamente na implementação do código e na execução dos testes, sem salvar checkpoints intermediários. O repositório Git foi criado apenas nesta entrega, com um commit contendo o estado completo das missões M01 a M04. Por isso, deixamos desmarcados os itens de commit/checkpoint dos checklists da M02, M03 e M04, já que não foram realizados no momento previsto. A partir da M06 e as demais entregas iremos nos atentar melhor e faremos um commit antes de começar e outro ao concluir cada missão.
- **Próxima ação:** Inserção ordenada e ampliação dos testes (M05).
## Missão M05 — 23/11
- **Integrantes e papéis:** André Luiz de Souza Justino (Implementação de `lse_inserir_ordenado` e escrita do `test_m05.c`) e José Henrique da Cruz Costa (Revisão, documentação e checklist).
- **Objetivo:** Implementar a inserção ordenada genérica e estável, mantendo `t_elem` privado e `tamanho` correto.
- **O que implementamos:** `lse_inserir_ordenado` em `src/lse.c` e os testes de `missoes/M05/test_m05.c` (lista vazia, início, meio, fim, empates estáveis com registros `musica_t`, argumentos inválidos e remoção seguida de reinserção).
- **Decisão técnica e justificativa:** O laço avança enquanto `comparar(atual, nova) <= 0`; o uso de `<=` (e não `<`) garante a estabilidade, pois elementos de mesma chave ficam na ordem de chegada. Argumentos inválidos são tratados de forma defensiva (retorno `0`), seguindo o padrão da M04.
- **Problema encontrado e solução:** A primeira versão da função precisou de ajustes (commits "M5 - lse_inserir_ordenado implementada" e "M05: lse_inserir_ordenado corrigida!"). Revisamos o religamento dos ponteiros `ant`/`novo`/`cam` nos casos de início e fim até que todos os cenários do `test_m05` passassem.
- **Reflexões da missão:**
  - *Por que inserir no início é O(1) e a ordenada é O(n)?* No início só mudam `novo->prox` e `inicio`; na ordenada é preciso percorrer até achar a posição, que no pior caso é o fim.
  - *Risco de critérios diferentes na mesma lista:* a pré-condição de lista ordenada deixa de valer e as inserções seguintes ficam em posições sem sentido.
  - *Comparador:* permite ordenar por título, duração ou `id` apenas trocando o callback, sem mudar `lse.c`.
  - *Chaves iguais:* mantêm a ordem relativa de chegada (estabilidade).
- **Evidências de testes:** `make test_m05` -> `M05: todos os testes passaram.`; regressão `make test_m03` e `make test_m04` aprovada.
- **Controle de versão:** checkpoint antes de iniciar (commit "Checkpoint inicial antes de implementar M05") e commit de conclusão da M05 com esta documentação.
- **Próxima ação:** Implementar a pilha como especialização da LSE (M06 — 25/11).
