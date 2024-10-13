%{
    #include<stdio.h>
int yylex(void);
void yyerror (char const *mensagem);
int get_line_number(void);
%}
%error-verbose

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_ERRO

%%

//Organizar isso aqui

/* Um programa na linguagem é composto por uma
lista de funções, sendo esta lista opcional */
programa:lista_de_funcoes | /*Isso aqui é vazio */ ;

lista_de_funcoes: lista_de_funcoes funcao | funcao ;

/* Cada função é definida por um cabeçalho e um
corpo.*/
funcao : cabecalho corpo

/* O cabeçalho consiste no nome da função,
o caractere igual ’=’, uma lista de parâmetros, o
operador maior ’>’ e o tipo de retorno. O tipo da
função pode ser float ou int */
cabecalho: TK_IDENTIFICADOR '=' lista_de_parametros  '>' tipo_de_retorno

/* A lista de parâmetros é composta por zero ou mais parâmetros de
entrada, separados por TK_OC_OR*/
lista_de_parametros: parametro | parametro TK_OC_OR parametro | /*Isso aqui é vazio*/;

/* Cada parâmetro é definido pelo seu nome seguido do 
caractere menor ’<’, seguido do caractere menos ’-’, seguido do tipo.  */
parametro: TK_IDENTIFICADOR '<' '-' tipo_de_retorno

/*O tipo da função pode ser float ou int*/
tipo_de_retorno: TK_PR_INT | TK_PR_FLOAT

/*O corpo da função é um bloco de comandos.*/
corpo :  bloco_de_comandos

/*Um bloco de comandos é definido entre chaves,e consiste em uma sequência, possivelmente vazia, 
de comandos simples cada um terminado por ponto-e-vírgula. */
bloco_de_comandos : '{' comando '}'

/*Um bloco de comandos é considerado como um comando único simples, 
recursivamente, e pode ser utilizado em qualquer construção que aceite 
um comando simples.*/
comando : bloco_de_comandos | comando_simples | /*Isso aqui é vazio*/;

comando_simples : TK_PR_INT

/*

Para programar a precedencia usando a gramatica, usa uma arvore, quanto mais baixo, maior a preferencia

Mais precencia mais embaixo
expr : expr '+' term;
term : term '*' factor | factor;
factor: '(' expr ') } | ID | NUM;

Fator tem mais precencia que term, que tem mais precedencia expr

*/


%%

void yyerror(char const *mensagem){
    fprintf(stderr,"%s at line %d\n",mensagem,get_line_number());
}