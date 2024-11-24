/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "symbol_table.h"
#include <stdio.h>

int hash (char *lexema)
{
    int hash = 0;

    for (int i = 0; lexema[i] != '\0'; i++)
        hash = 31 * hash + lexema[i];

    return hash % HASH_SIZE;
}

symbol_table_t *create_symbol_table()
{
    symbol_table_t *table = (symbol_table_t *)malloc(sizeof(symbol_table_t));

    for (int i = 0; i < HASH_SIZE; i++)
        table[i].next = NULL;

    return table;
}

void destroy_symbol_table(symbol_table_t *table)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        symbol_table_t *current = table[i].next;
        while (current != NULL)
        {
            symbol_table_t *next = current->next;
            free(current->lexema);
            free(current);
            current = next;
        }
    }

    free(table);
}

symbol_table_t *insert_element(symbol_table_t *table, char *lexema, content_t *content)
{
    int index = hash(lexema);

    symbol_table_t *new = (symbol_table_t *)malloc(sizeof(symbol_table_t));
    new->lexema = strdup(lexema);
    new->content = content;

    new->next = table[index].next;
    table[index].next = new;

    return table;
}

symbol_table_t *remove_element(symbol_table_t *table, char *lexema)
{
    int index = hash(lexema);

    symbol_table_t *current = &table[index];
    while (current->next != NULL)
    {
        if (strcmp(current->next->lexema, lexema) == 0)
        {
            symbol_table_t *next = current->next->next;
            free(current->next->lexema);
            free(current->next);
            current->next = next;
            break;
        }
        current = current->next;
    }

    return table;
}

content_t *search_table(symbol_table_t *table, char *lexema)
{
    int index = hash(lexema);

    symbol_table_t *current = table[index].next;
    fprintf(stdout,"Current is %p\n",current);
    while (current != NULL)
    {
        if (strcmp(current->lexema, lexema) == 0)
            return current->content;
        current = current->next;
    }

    return NULL;
}
