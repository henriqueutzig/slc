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

#define HASH_SIZE 999

typedef struct symbol_table
{
    char *lexema;
    content_t *content;
    struct symbol_table *next;
} symbol_table_t;

int hash (char *lexema);

symbol_table_t *create_symbol_table();
void destroy_symbol_table(symbol_table_t *table);   

symbol_table_t *insert(symbol_table_t *table, char *lexema, content_t *content);
symbol_table_t *remove(symbol_table_t *table, char *lexema);

content_t *search(symbol_table_t *table, char *lexema);

#endif