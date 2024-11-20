#include "content.h" 

content_t *create_content(int line, lexema *value, type_t type)
{
    content_t *content = (content_t *)malloc(sizeof(content_t));

    content->line = line;
    content->value = value;
    content->type = type;
    
    return content;
}