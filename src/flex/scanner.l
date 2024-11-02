/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/
%{
	#include "bison/parser.tab.h"

	static int line_number = 1;
	static int column_number = 1;
	int get_line_number (void); 
	int get_column_number(void);
%}

%option noyywrap

/* Regular expression definitions */
digit        [0-9]
number       {digit}+
lower_char	 [a-z]

%% /* Regex definitions */

"//".*        								{  }
[ \t]										{ column_number += yyleng; }

"int"                        				{ column_number += yyleng; return TK_PR_INT; }
"float"										{ column_number += yyleng; return TK_PR_FLOAT; }
"if"										{ column_number += yyleng; return TK_PR_IF; }
"else"										{ column_number += yyleng; return TK_PR_ELSE; }
"while"										{ column_number += yyleng; return TK_PR_WHILE; }
"return"									{ column_number += yyleng; return TK_PR_RETURN; }

"<="										{ column_number += yyleng; return TK_OC_LE; }
">="										{ column_number += yyleng; return TK_OC_GE; }
"=="										{ column_number += yyleng; return TK_OC_EQ; }
"!="										{ column_number += yyleng; return TK_OC_NE; }
"&"											{ column_number += yyleng; return TK_OC_AND; }
"|"											{ column_number += yyleng; return TK_OC_OR; }

({lower_char}|_)({lower_char}|_|{digit})* 	{ yylval.lex_value = create_lexema(yylineno, yytext, IDENTIFICADOR); column_number += yyleng; return TK_IDENTIFICADOR; }

{number}									{ yylval.lex_value = create_lexema(yylineno, yytext, LITERAL); column_number += yyleng; return TK_LIT_INT; }
{number}?\.{number}							{ yylval.lex_value = create_lexema(yylineno, yytext, LITERAL); column_number += yyleng; return TK_LIT_FLOAT; }

[\!\*\/\%\+\-\<\>\{\}\(\)\=\,\;]			{ column_number += yyleng; return (int)yytext[0]; }

\n                        					{ line_number++; column_number = 1; }
.											{ column_number += yyleng; return TK_ERRO; }

%% /* C functions */

int get_line_number(void) {
	return line_number;
}

int get_column_number(void) {
	return column_number;
}