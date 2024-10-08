/*
Função principal para realização da análise sintática.

Este arquivo será posterioremente substituído, não acrescente nada.
*/
#include <stdio.h>
#include "bison/parser.tab.h" //arquivo gerado com bison -d parser.y

extern int yylex_destroy(void);
extern int yyparse();

int main (int argc, char **argv)
{
  int ret = yyparse();
  yylex_destroy();
  return ret;
}