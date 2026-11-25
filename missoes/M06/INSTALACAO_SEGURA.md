# Instalação M06
Na raiz do projeto, faça backup, confira o conteúdo do ZIP e extraia sem sobrescrever:

```sh
unzip -l MiniPlayer_M06_Incremental.zip
unzip -n MiniPlayer_M06_Incremental.zip -d .
make test_m06
make test_missoes
```

Se a integração automática ainda não estiver configurada, siga o README de Makefiles incrementais já distribuído ou execute `make -f missoes/M06/Makefile.incremental test_m06`. Não inclua o mesmo arquivo duas vezes. Se M06 já existir, confira colisões antes de extrair. Os stubs existentes podem compilar e falhar nas asserções até serem implementados. Não substitua código anterior.
