# Live Reload

1. Instalar `entr`: `brew install entr`
2. Rodar `find ./src ./tests -type f | entr -c make test`

Cada vez que salvar um arquiva em src ou test, os testes rodam de novo.
