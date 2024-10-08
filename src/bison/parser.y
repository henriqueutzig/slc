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

programa:lista_de_funcoes | /*Isso aqui é vazio */ ;
lista_de_funcoes: lista_de_funcoes funcao | funcao ;
funcao : TK_PR_INT

%%

void yyerror(char const *mensagem){
    fprintf(stderr,"%s \n",mensagem);
}