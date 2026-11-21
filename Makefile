CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude
BUILD = build
APP = $(BUILD)/miniplayer
.PHONY: all test clean
all: $(APP)
$(BUILD):
	mkdir -p $(BUILD)
$(APP): src/main.c src/miniplayer.c include/miniplayer.h | $(BUILD)
	$(CC) $(CFLAGS) src/main.c src/miniplayer.c -o $@
$(BUILD)/test_musica: tests/test_musica.c src/musica.c | $(BUILD)
	$(CC) $(CFLAGS) $^ -o $@
$(BUILD)/test_lse: tests/test_lse.c src/lse.c | $(BUILD)
	$(CC) $(CFLAGS) $^ -o $@
$(BUILD)/test_pilha: tests/test_pilha.c src/pilha.c src/lse.c | $(BUILD)
	$(CC) $(CFLAGS) $^ -o $@
$(BUILD)/test_fila: tests/test_fila.c src/fila.c src/lse.c | $(BUILD)
	$(CC) $(CFLAGS) $^ -o $@
test: $(BUILD)/test_musica $(BUILD)/test_lse $(BUILD)/test_pilha $(BUILD)/test_fila
	@set -e; for t in $(BUILD)/test_musica $(BUILD)/test_lse $(BUILD)/test_pilha $(BUILD)/test_fila; do echo "== $$t =="; ./$$t; done
clean:
	rm -rf $(BUILD)

	#-----------------------------------------------------------
# MiniPlayer: testes incrementais das missões
# -----------------------------------------------------------

# Descobre automaticamente as missões instaladas.
ARQUIVOS_MISSOES := $(wildcard missoes/M*/Makefile.incremental)

# Define os nomes dos testes, inclusive o da M02.
TESTES_MISSOES := $(patsubst missoes/M%/Makefile.incremental,test_m%,$(ARQUIVOS_MISSOES))

# Incorpora as regras de cada missão, sem alterar seus arquivos.
-include $(ARQUIVOS_MISSOES)

.PHONY: test_missoes

# Executa todos os testes das missões instaladas.
test_missoes: $(TESTES_MISSOES)
