#include "lexema.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

lexema *create_lexema(int linha, char *valor, tipo tipo){

    lexema *lex = (lexema*)malloc(sizeof(lexema));

    lex->tipo=tipo;
    lex->linha=linha;
    lex->valor=valor;

    return lex;
}