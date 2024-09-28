%{
	#include "../../includes/tokens/tokens.h"

	static int line_number = 1;
	int get_line_number (void); 
%}

/* Regular expression definitions */
digit        [0-9]
number       [0-9]+
char         [a-zA-Z_]
alphanumeric  [a-zA-Z0-9_]

%x COMMENT

%% /* Regex definitions */

"int"                       { return TK_PR_INT; }
"float"						{ return TK_PR_FLOAT; }
"if"						{ return TK_PR_IF; }
"else"						{ return TK_PR_ELSE; }
"while"						{ return TK_PR_WHILE; }
"return"					{ return TK_PR_RETURN; }

"<="						{ return TK_OC_LE; }
">="						{ return TK_OC_GE; }
"=="						{ return TK_OC_EQ; }
"!="						{ return TK_OC_NE; }
"&"							{ return TK_OC_AND; }
"|"							{ return TK_OC_OR; }

'\n'                        { line_number++; } // LineCounter

%% /* C functions */

int get_line_number(void) {
	return line_number;
}
