/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/
%{
    #include<stdio.h>
    int yylex(void);
    void yyerror (char const *mensagem);
    int get_line_number(void);
    int get_column_number(void);
%}

%define parse.error verbose

%code requires { #include "asd.h" }
%union {
  double *lex_value;  // TODO: create a value type struct
  asd_tree_t *tree;
}

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
%token<lex_value> TK_IDENTIFICADOR
%token<lex_value> TK_LIT_INT
%token<lex_value> TK_LIT_FLOAT
%token TK_ERRO

%type <tree> programa
%type <tree> lista_de_funcoes
%type <tree> funcao
%type <tree> cabecalho
%type <tree> corpo
%type <tree> lista_de_parametros
%type <tree> tipos_de_variavel
%type <tree> parametro
%type <tree> bloco_de_comandos
%type <tree> comando
%type <tree> comando_simples
%type <tree> chamada_de_funcao
%type <tree> declaracao_variavel
%type <tree> atribuicao_variavel
%type <tree> comando_de_retorno
%type <tree> fluxo_if
%type <tree> fluxo_while
%type <tree> expressao
%type <tree> expressao_precedencia_6
%type <tree> expressao_precedencia_5
%type <tree> expressao_precedencia_4
%type <tree> expressao_precedencia_3
%type <tree> expressao_precedencia_2
%type <tree> expressao_precedencia_1
%type <tree> operando
%type <tree> literal
%type <tree> variavel
%type <tree> argumento

%%

/* 
    Um programa na linguagem é composto por uma
    lista de funções, sendo esta lista opcional 
*/
programa: lista_de_funcoes 
    | %empty;

lista_de_funcoes: lista_de_funcoes funcao 
    | funcao;

/* 
    Cada função é definida por um cabeçalho e um corpo.
*/
funcao: cabecalho corpo;

/* 
    O cabeçalho consiste no nome da função,
    o caractere igual ’=’, uma lista de parâmetros, o
    operador maior ’>’ e o tipo de retorno. O tipo da
    função pode ser float ou int 
*/
cabecalho: TK_IDENTIFICADOR '=' lista_de_parametros  '>' tipos_de_variavel 
    | TK_IDENTIFICADOR '=''>' tipos_de_variavel;

/* 
    A lista de parâmetros é composta por zero ou mais parâmetros de
    entrada, separados por TK_OC_OR
*/
lista_de_parametros: parametro 
    | lista_de_parametros TK_OC_OR parametro;

/* 
    Cada parâmetro é definido pelo seu nome seguido do 
    caractere menor ’<’, seguido do caractere menos ’-’, seguido do tipo.  
*/
parametro: TK_IDENTIFICADOR '<' '-' tipos_de_variavel;

/*
    O tipo da função pode ser float ou int
*/
tipos_de_variavel: TK_PR_INT 
    | TK_PR_FLOAT;

/*
    O corpo da função é um bloco de comandos.
*/
corpo: bloco_de_comandos;

/*
    Um bloco de comandos é definido entre chaves, e consiste em uma 
    sequência, possivelmente vazia, de comandos simples cada um 
    expressao_precedencia_2inado por ponto-e-vírgula. 
*/
bloco_de_comandos: 
    '{' comando ';' '}' 
    | '{''}';

/*
    Um bloco de comandos é considerado como um comando único simples, 
    recursivamente, e pode ser utilizado em qualquer construção que aceite 
    um comando simples.
*/
comando: comando ';' comando_simples
    | comando_simples

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
    | bloco_de_comandos
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

lista_de_variaveis: 
    variavel_inicializada                          
    | lista_de_variaveis ',' variavel_inicializada ;

variavel_inicializada : 
    variavel                    
    | variavel TK_OC_LE literal ;

variavel: TK_IDENTIFICADOR;

literal: 
    TK_LIT_FLOAT { $$ = asd_new($1->value) }
    | TK_LIT_INT { $$ = asd_new($1->value) };

/*
    O comando de atribuição consiste em um identificador seguido 
    pelo caractere de igualdade seguido por uma expressão
*/
atribuicao_variavel: TK_IDENTIFICADOR '=' expressao { $$ = asd_new("="); asd_add_child($$, asd_new($1->value)); asd_add_child($$, $3); };

/*
    Uma chamada de função consiste no nome da função, seguida de 
    argumentos entre parênteses separados por vírgula. 
    Um argumento pode ser uma expressão.
*/
chamada_de_funcao: TK_IDENTIFICADOR '(' argumento ')' { $$ = asd_new($1->value); asd_add_child($$, $3); };

argumento: 
    argumento ',' expressao { $$ = $1; asd_add_child($$, $3); }
    | expressao             { $$ = $1; };
/*
    Trata-se do token return seguido de uma expressão
*/
comando_de_retorno: TK_PR_RETURN expressao { $$ = asd_new("return"); asd_add_child($$, $2); };

/*
    A condicional consiste no token if seguido de uma 
    expressão entre parênteses e então por um bloco de
    comandos obrigatório. O else, sendo opcional, 
    deve sempre aparecer após o bloco do if, e 
    é seguido de um bloco de comandos, 
    obrigatório caso o else seja empregado.
*/
fluxo_if: 
    TK_PR_IF '(' expressao ')' bloco_de_comandos { $$ = asd_new("if"); asd_add_child($$, $3); asd_add_child($$, $5); }
    | TK_PR_IF '(' expressao ')' bloco_de_comandos TK_PR_ELSE bloco_de_comandos { $$ = asd_new("if"); asd_add_child($$, $3); asd_add_child($$, $5); asd_add_child($$, $7); };

/*
    Temos apenas uma construção de repetição que é o token while seguido
    de uma expressão entre parênteses e de um bloco de comandos
*/
fluxo_while: TK_PR_WHILE '(' expressao ')' bloco_de_comandos {$$ = asd_new("while"); asd_add_child($$, $3); asd_add_child($$, $5); };

/*
    Expressoes conforme definidas na tabela na especificaça da E2
    Quanto mais baixo, maior a precedencia, de maneira que a 
    precedencia 6 é precedida por operaçoes do nivel 1
*/
expressao: 
    expressao TK_OC_OR expressao_precedencia_6 { $$ = asd_new("|"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_6                  { $$ = $1; };

expressao_precedencia_6: 
    expressao_precedencia_6 TK_OC_AND expressao_precedencia_5 { $$ = asd_new("&"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_5                                 { $$ = $1; };

expressao_precedencia_5: 
    expressao_precedencia_5 TK_OC_EQ expressao_precedencia_4   { $$ = asd_new("=="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_5 TK_OC_NE expressao_precedencia_4 { $$ = asd_new("!="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_4                                  { $$ = $1; };

expressao_precedencia_4:
    expressao_precedencia_4 '<' expressao_precedencia_3        { $$ = asd_new("<"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_4 '>' expressao_precedencia_3      { $$ = asd_new(">"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_4 TK_OC_LE expressao_precedencia_3 { $$ = asd_new("<="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_4 TK_OC_GE expressao_precedencia_3 { $$ = asd_new(">="); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_3                                  { $$ = $1; };

expressao_precedencia_3:
    expressao_precedencia_3 '+' expressao_precedencia_2   { $$ = asd_new("+"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_3 '-' expressao_precedencia_2 { $$ = asd_new("-"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_2                             { $$ = $1; };

expressao_precedencia_2: 
    expressao_precedencia_2 '*' expressao_precedencia_1   { $$ = asd_new("*"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_2 '/' expressao_precedencia_1 { $$ = asd_new("/"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_2 '%' expressao_precedencia_1 { $$ = asd_new("%"); asd_add_child($$, $1); asd_add_child($$, $3); }
    | expressao_precedencia_1                             { $$ = $1 };

expressao_precedencia_1: 
    '-' expressao_precedencia_1   { $$ = asd_new("-"); asd_add_child($$, $2); }
    | '!' expressao_precedencia_1 { $$ = asd_new("!"); asd_add_child($$, $2); }
    | operando                    { $$ = $1; };

/*
Os operandos podem ser (a) iden-
tificadores, (b) literais e (c) chamada de função ou
(d) outras expressões
*/
operando: 
    TK_IDENTIFICADOR    { $$ = asd_new($1->value); }
    | literal           { $$ = $1; }
    | chamada_de_funcao { $$ = $1; }
    | '('expressao')'   { $$ = $2; };


%%

void yyerror(char const *mensagem){
    fprintf(stderr,"%s at line %d column %d/n",mensagem,get_line_number(),get_column_number());
}