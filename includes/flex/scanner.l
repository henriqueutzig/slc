%{
	#include "../includes/tokens/tokens.h"

	int LineNumber = 1;
%}

%x COMMENT

%%

int                         {return TK_PR_INT;       }