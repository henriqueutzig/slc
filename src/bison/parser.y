%{
    #include<stdio.h>
int yylex(void);
void yyerror (char const *mensagem);
%}

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

programa:lista_de_funcoes | /*Isso aqui é vazio */ ;
lista_de_funcoes: lista_de_funcoes funcao | funcao ;
funcao : cabecalho corpo
cabecalho: TK_IDENTIFICADOR '=' lista_de_parametros  '>' tipo_de_retorno
lista_de_parametros: parametro | parametro TK_OC_OR parametro
parametro: TK_IDENTIFICADOR '<' '-' tipo_de_retorno
tipo_de_retorno: TK_PR_INT | TK_PR_FLOAT
corpo :  bloco_de_comandos
bloco_de_comandos : '{' comando '}'
comando : bloco_de_comandos | TK_PR_INT

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
    fprintf(stderr,"%s \n",mensagem);
}