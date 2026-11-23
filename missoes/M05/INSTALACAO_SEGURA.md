# Instalação segura — Missão 05

Faça backup

Copie o ZIP para a raiz do projeto, confira o conteúdo e extraia sem sobrescrever:

```sh
unzip -l MiniPlayer_M05_Incremental.zip
unzip -n MiniPlayer_M05_Incremental.zip -d .
```

**Não substitua** `src/lse.c`, `include/lse.h`, `Makefile` ou documentos já existentes. A missão só instala arquivos sob `missoes/M05/`.

## Inclusão automática no Makefile principal

Se ainda não estiver configurado, acrescente **uma única vez** ao final do Makefile principal:

```makefile
ARQUIVOS_MISSOES := $(wildcard missoes/M*/Makefile.incremental)
TESTES_MISSOES := $(patsubst missoes/M%/Makefile.incremental,test_m%,$(ARQUIVOS_MISSOES))
-include $(ARQUIVOS_MISSOES)
.PHONY: test_missoes
test_missoes: $(TESTES_MISSOES)
```

Se o Makefile principal já inclui `missoes/M*/Makefile.incremental`, **não acrescente outro bloco**.

## Executar

```sh
make test_m05
make test_m03
make test_m04
make test_missoes
```

Sem integração no Makefile principal:

```sh
make -f missoes/M05/Makefile.incremental test_m05
```

O teste usa `lse_percorrer`, introduzida na M03. Se surgir erro de declaração implícita, confira se `lse_percorrer` está declarada em `include/lse.h` e implementada em `src/lse.c`, conforme o trabalho da M03. Se a implementação atual não compilar, isso é esperado até concluir a missão.
