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
    table->content = NULL;
    for (int i = 0; i < HASH_SIZE; i++)
        table[i].next = NULL;

    return table;
}

void destroy_symbol_table(symbol_table_t *table)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (table[i].next == NULL) continue;

        symbol_table_t *current = table[i].next;

        fprintf(stderr, "Destroying table %d\n", i);
        while (current != NULL)
        {
            fprintf(stderr, "Current: %p\n", current);
            fprintf(stderr, "Current->lexema: %s\n", current->content_lexema_value);
            symbol_table_t *next = current->next;
            fprintf(stderr, "Next: %p\n", next);
            if (current->content_lexema_value != NULL)
                free(current->content_lexema_value);
            if (current->content != NULL)
                free(current->content);
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
    new->content_lexema_value = strdup(lexema);
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
        if (strcmp(current->next->content_lexema_value, lexema) == 0)
        {
            symbol_table_t *next = current->next->next;
            free(current->next->content_lexema_value);
            free(current->next);
            current->next = next;
            break;
        }
        current = current->next;
    }

    return table;
}

content_t *search_table(symbol_table_t *table, char *lexema) {
    int index = hash(lexema); 

    symbol_table_t *current = table[index].next; 
    while (current != NULL) {
        if (strcmp(current->content_lexema_value, lexema) == 0) {
            return current->content; 
        }
        current = current->next;
    }
    return NULL; 
}