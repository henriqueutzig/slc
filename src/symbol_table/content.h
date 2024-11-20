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

typedef enum {
    INT = 0,
    FLOAT
} type_t;

typedef struct content_t
{
    int line;
    type_t type;
    lexema *value;
} content_t;

content_t *create_content(int line, lexema *value, type_t type);

#endif