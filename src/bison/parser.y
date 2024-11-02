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

    extern void* arvore;
%}

%define parse.error verbose

%code requires { #include "asd.h" }
%code requires { #include "lexema.h" }
%union {
  lexema *valor;
  asd_tree_t *arvore;
}


%token<valor> TK_PR_INT
%token<valor> TK_PR_FLOAT
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
%token<valor> TK_IDENTIFICADOR
%token<valor> TK_LIT_INT
%token<valor> TK_LIT_FLOAT
%token TK_ERRO

%type <arvore> programa
%type <arvore> lista_de_funcoes
%type <arvore> funcao
%type <arvore> cabecalho
%type <arvore> corpo
%type <arvore> lista_de_parametros
%type <arvore> tipos_de_variavel
%type <arvore> parametro
%type <arvore> bloco_de_comandos
%type <arvore> comando
%type <arvore> comando_simples
%type <arvore> chamada_de_funcao
%type <arvore> declaracao_variavel
%type <arvore> atribuicao_variavel
%type <arvore> comando_de_retorno
%type <arvore> fluxo_if
%type <arvore> fluxo_while
%type <arvore> expressao
%type <arvore> expressao_precedencia_6
%type <arvore> expressao_precedencia_5
%type <arvore> expressao_precedencia_4
%type <arvore> expressao_precedencia_3
%type <arvore> expressao_precedencia_2
%type <arvore> expressao_precedencia_1
%type <arvore> operando
%type <arvore> literal
%type <arvore> variavel
%type <arvore> argumento
%type <arvore> variavel_inicializada
%type <arvore> lista_de_variaveis


%%

/* 
    Um programa na linguagem é composto por uma
    lista de funções, sendo esta lista opcional 
*/
programa: 
    lista_de_funcoes { if ($1 != NULL) $$ = $1; else $$ = NULL; arvore = $$; };
    | %empty         { $$ = NULL;};

lista_de_funcoes: 
    lista_de_funcoes funcao {$$ = $1; asd_add_child($$,$2);}
    | funcao {$$=$1;};

/* 
    Cada função é definida por um cabeçalho e um corpo.
*/
funcao: cabecalho corpo {$$ = asd_new("func");asd_add_child($$,$1);asd_add_child($$,$2);};

/* 
    O cabeçalho consiste no nome da função,
    o caractere igual ’=’, uma lista de parâmetros, o
    operador maior ’>’ e o tipo de retorno. O tipo da
    função pode ser float ou int 
*/
cabecalho: TK_IDENTIFICADOR '=' lista_de_parametros  '>' tipos_de_variavel {$$ = asd_new("cabecalho");asd_add_child($$,$3);asd_add_child($$,$5);}
    | TK_IDENTIFICADOR '=''>' tipos_de_variavel {$$ = asd_new("cabecalho");asd_add_child($$,$4);};

/* 
    A lista de parâmetros é composta por zero ou mais parâmetros de
    entrada, separados por TK_OC_OR
*/
lista_de_parametros: parametro  {$$=$1;}
    | lista_de_parametros TK_OC_OR parametro {$$ = asd_new("|");asd_add_child($$,$1);asd_add_child($$,$3);};

/* 
    Cada parâmetro é definido pelo seu nome seguido do 
    caractere menor ’<’, seguido do caractere menos ’-’, seguido do tipo.  
*/
parametro: TK_IDENTIFICADOR '<' '-' tipos_de_variavel {$$=asd_new("<-");asd_add_child($$,asd_new($1->valor));asd_add_child($$, $4);};

/*
    O tipo da função pode ser float ou int
*/
tipos_de_variavel: TK_PR_INT  { $$ = asd_new($1->valor); }
    | TK_PR_FLOAT  { $$ = asd_new($1->valor); };

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
    '{' comando ';' '}' {$$ = $2;}
    | '{''}' {$$ = NULL;};

/*
    Um bloco de comandos é considerado como um comando único simples, 
    recursivamente, e pode ser utilizado em qualquer construção que aceite 
    um comando simples.
*/
comando: comando ';' comando_simples {$$ = $1; asd_add_child($$,$1);}
    | comando_simples {$$ = $1;}

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
    variavel_inicializada  {$$ = $1;}
    | lista_de_variaveis ',' variavel_inicializada {if($1 != NULL) {$$ = $1; if($3 != NULL) asd_add_child($1,$3);}} ;

variavel_inicializada : 
    variavel
    | variavel TK_OC_LE literal {$$ = asd_new("<=");asd_add_child($$,$1); asd_add_child($$,$3);} ;

variavel: TK_IDENTIFICADOR  { $$ = asd_new($1->valor); };

literal: 
    TK_LIT_FLOAT { $$ = asd_new($1->valor);}
    | TK_LIT_INT { $$ = asd_new($1->valor);};

/*
    O comando de atribuição consiste em um identificador seguido 
    pelo caractere de igualdade seguido por uma expressão
*/
atribuicao_variavel: TK_IDENTIFICADOR '=' expressao { $$ = asd_new("="); asd_add_child($$, asd_new($1->valor)); asd_add_child($$, $3); };

/*
    Uma chamada de função consiste no nome da função, seguida de 
    argumentos entre parênteses separados por vírgula. 
    Um argumento pode ser uma expressão.
*/
chamada_de_funcao: TK_IDENTIFICADOR '(' argumento ')' { $$ = asd_new($1->valor); asd_add_child($$, $3); };

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
    | expressao_precedencia_1                             { $$ = $1; };

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
    TK_IDENTIFICADOR    { $$ = asd_new($1->valor); }
    | literal           { $$ = $1; }
    | chamada_de_funcao { $$ = $1; }
    | '('expressao')'   { $$ = $2; };


%%

void yyerror(char const *mensagem){
    fprintf(stderr,"%s at line %d column %d/n",mensagem,get_line_number(),get_column_number());
}