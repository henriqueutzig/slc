/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "content.h" 
#include <stdio.h>

content_t *create_content(int line, lexema *value, type_t type, nature_t nature, char *base_reg, unsigned int offset)
{
    content_t *content = (content_t *)malloc(sizeof(content_t));

    content->line = line;
    content->lexema = value;
    content->type = type;
    content->nature = nature;

    content->offset = offset;
    content->base_reg = base_reg;
    
    return content;
}