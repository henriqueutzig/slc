/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#ifndef _CONTENT_H_
#define _CONTENT_H_

#include <stdlib.h>
#include "../lexema/lexema.h"

typedef enum type_t {
    INT = 0,
    FLOAT
} type_t;

typedef enum nature_t {
    VARIABLE = 0,
    FUNCTION
} nature_t;

typedef struct content_t
{
    int line;
    type_t type;
    nature_t nature;
    lexema *lexema;
} content_t;

content_t *create_content(int line, lexema *value, type_t type, nature_t nature);

#endif