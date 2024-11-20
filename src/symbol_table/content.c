/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "content.h" 

content_t *create_content(int line, lexema *value, type_t type)
{
    content_t *content = (content_t *)malloc(sizeof(content_t));

    content->line = line;
    content->value = value;
    content->type = type;
    
    return content;
}