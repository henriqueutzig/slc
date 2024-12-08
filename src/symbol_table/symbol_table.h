/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include "content.h"
#include <stdlib.h> 
#include <string.h>
#include <assert.h>

#define HASH_SIZE 999

typedef struct symbol_table
{
    char *content_lexema_value;
    content_t *content;
    struct symbol_table *next;
} symbol_table_t;

unsigned hash (char *lexema);

symbol_table_t *create_symbol_table();
void destroy_symbol_table(symbol_table_t *table);   

symbol_table_t *insert_element(symbol_table_t *table, char *lexema, content_t *content);
symbol_table_t *remove_element(symbol_table_t *table, char *lexema);

content_t *search_table(symbol_table_t *table, char *lexema);
unsigned int get_offset(symbol_table_t *table, type_t var_type);

#endif