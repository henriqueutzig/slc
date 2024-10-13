%{
    #include<stdio.h>
int yylex(void);
void yyerror (char const *mensagem);
int get_line_number(void);
int get_column_number(void);
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

/* 
    Um programa na linguagem é composto por uma
    lista de funções, sendo esta lista opcional 
*/
programa: lista_de_funcoes 
    | /*Isso aqui é vazio */ ;

lista_de_funcoes: lista_de_funcoes funcao 
    | funcao ;

/* 
    Cada função é definida por um cabeçalho e um corpo.
*/
funcao: cabecalho corpo

/* 
    O cabeçalho consiste no nome da função,
    o caractere igual ’=’, uma lista de parâmetros, o
    operador maior ’>’ e o tipo de retorno. O tipo da
    função pode ser float ou int 
*/
cabecalho: TK_IDENTIFICADOR '=' lista_de_parametros  '>' tipos_de_variavel

/* 
    A lista de parâmetros é composta por zero ou mais parâmetros de
    entrada, separados por TK_OC_OR
*/
lista_de_parametros: parametro | lista_de_parametros TK_OC_OR parametro | /*Isso aqui é vazio*/;

/* 
    Cada parâmetro é definido pelo seu nome seguido do 
    caractere menor ’<’, seguido do caractere menos ’-’, seguido do tipo.  
*/
parametro: TK_IDENTIFICADOR '<' '-' tipos_de_variavel

/*
    O tipo da função pode ser float ou int
*/
tipos_de_variavel: TK_PR_INT | TK_PR_FLOAT

/*
    O corpo da função é um bloco de comandos.
*/
corpo: bloco_de_comandos

/*
    Um bloco de comandos é definido entre chaves, e consiste em uma 
    sequência, possivelmente vazia, de comandos simples cada um 
    expressao_2inado por ponto-e-vírgula. 
*/
bloco_de_comandos: '{' comando '}'

/*
    Um bloco de comandos é considerado como um comando único simples, 
    recursivamente, e pode ser utilizado em qualquer construção que aceite 
    um comando simples.
*/
comando: comando comando 
    | comando_simples ';' 
    | /*Isso aqui é vazio*/;

/*
    Os comandos simples da linguagem podem ser:
    declaração de variável, atribuição, construções de
    fluxo de controle, operação de retorno, um bloco
    de comandos, e chamadas de função.
*/
comando_simples: chamada_de_funcao 
    | declaracao_variavel 
    | atribuicao_variavel 
    | comando_de_retorno 
    | fluxo_if 
    | fluxo_while;

/*
    Consiste no tipo da variável seguido de uma lista composta de pelo
    menos um nome de variável (identificador) separa-
    das por vírgula. Os tipos podem ser int e float.
    Uma variável pode ser opcionalmente inicializada
    caso sua declaração seja seguida do operador com-
    posto TK_OC_LE e de um literal.
*/
declaracao_variavel: tipos_de_variavel lista_de_variaveis
lista_de_variaveis: variavel 
    | variavel ',' lista_de_variaveis;
variavel: TK_IDENTIFICADOR 
    | TK_IDENTIFICADOR TK_OC_LE expressao;
literal: TK_LIT_FLOAT 
    | TK_LIT_INT;

/*
    O comando de atribuição consiste em um identificador seguido 
    pelo caractere de igualdade seguido por uma expressão
*/
atribuicao_variavel: TK_IDENTIFICADOR '=' expressao

/*
    Uma chamada de função consiste no nome da função, seguida de 
    argumentos entre parênteses separados por vírgula. 
    Um argumento pode ser uma expressão.
*/
chamada_de_funcao: TK_IDENTIFICADOR '(' argumento ')'
argumento: argumento ',' argumento 
    | expressao 
    | /*Isso é o vazio*/;

/*
    Trata-se do token return seguido de uma expressão
*/
comando_de_retorno: TK_PR_RETURN expressao

/*
    A condicional consiste no token if seguido de uma 
    expressão entre parênteses e então por um bloco de
    comandos obrigatório. O else, sendo opcional, 
    deve sempre aparecer após o bloco do if, e 
    é seguido de um bloco de comandos, 
    obrigatório caso o else seja empregado.
*/
fluxo_if: TK_PR_IF '(' expressao ')' bloco_de_comandos 
    | TK_PR_IF '(' expressao ')' bloco_de_comandos TK_PR_ELSE bloco_de_comandos

/*
    Temos apenas uma construção de repetição que é o token while seguido
    de uma expressão entre parênteses e de um bloco de comandos
*/
fluxo_while: TK_PR_WHILE '(' expressao ')' bloco_de_comandos

/*
    Expressoes conforme definidas na tabela na especificaça da E2
    Quanto mais baixo, maior a precedencia
*/
expressao: expressao TK_OC_OR expressao 
    | expressao_6;
expressao_6: expressao_6 TK_OC_AND expressao_6 
    | expressao_5;
expressao_5: expressao_5 TK_OC_EQ expressao_5 
    | expressao_5 TK_OC_NE expressao_5 
    | expressao_4;
expressao_4: expressao_4 '<' expressao_4 
    | expressao_4 '>' expressao_4 
    | expressao_4 TK_OC_LE expressao_4 
    | expressao_4 TK_OC_GE expressao_4 
    | expressao_3;
expressao_3:expressao_3 '+' expressao_3 
    | expressao_3 '-' expressao_3 
    | expressao_2;
expressao_2: expressao_2 '*' expressao_2 
    | expressao_2 '/' expressao_2 
    | expressao_2 '%' expressao_2 
    | expressao_1;
expressao_1: '-' expressao_terminal 
    | '!' expressao_terminal 
    | expressao_terminal;
expressao_terminal: literal 
    | TK_IDENTIFICADOR 
    | chamada_de_funcao 
    | '('expressao')';

/*
    Para programar a precedencia usando a gramatica, usa uma arvore, 
    quanto mais baixo, maior a preferencia

    Mais precedencia mais embaixo
    expr: expr '+' expressao_2;
    expressao_2: expressao_2 '*' factor | factor;
    factor: '(' expr ') } | ID | NUM;

    expressao_terminal tem mais precedencia que expressao_2, 
    que tem mais precedencia expr
*/


%%

void yyerror(char const *mensagem){
    fprintf(stderr,"%s at line %d\n column %d/n",mensagem,get_line_number(),get_column_number());
}