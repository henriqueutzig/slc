/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned hash(char *lexema)
{
    unsigned hash_value = 0;
    int i = 0;

    while (lexema[i] != '\0') {
        hash_value = (hash_value * 151) + lexema[i];
        i++;
    }

    return hash_value % HASH_SIZE;  
}

symbol_table_t *create_symbol_table()
{
    symbol_table_t *table = (symbol_table_t *)malloc(HASH_SIZE * sizeof(symbol_table_t));
    if (table == NULL) {
        fprintf(stderr, "Memory allocation failed for symbol table\n");
        exit(1);
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        table[i].content = NULL;
        table[i].next = NULL;
    }

    return table;
}

void destroy_symbol_table(symbol_table_t *table)
{
    assert(table != NULL);

    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (table[i].next == NULL) continue;

        symbol_table_t *current = table[i].next;

        while (current != NULL)
        {
            symbol_table_t *next = current->next;
            
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
    assert(table != NULL);
    int index = hash(lexema);

    symbol_table_t *new = (symbol_table_t *)malloc(sizeof(symbol_table_t));
    new->content_lexema_value = strdup(lexema);
    new->content = content;

    new->next = table[index].next;
    table[index].next = new;

    // fprintf(stderr, "Inserted '%s' at [%d] in table (%p)\n", lexema, index, table);

    return table;
}

symbol_table_t *remove_element(symbol_table_t *table, char *lexema)
{
    assert(table != NULL);
    int index = hash(lexema);

    symbol_table_t *current = &table[index];
    assert(current != NULL);
    while (current->next != NULL)
    {
        if (strcmp(current->next->content_lexema_value, lexema) == 0)
        {
            symbol_table_t *next = current->next->next;
            assert(next != NULL);
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
    assert(table != NULL);
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

unsigned int get_offset(symbol_table_t *table, type_t var_type) {
    assert(table != NULL);

    unsigned int var_type_size = var_type == INT ? INT_SIZE : FLOAT_SIZE;
    unsigned int offset = 0;
    symbol_table_t *last_instance = NULL;
    for (int i = 0; i < HASH_SIZE; i++) {
        symbol_table_t *current = table[i].next;
        while (current != NULL) {
            if (current->content->type == var_type) {
                last_instance = current;
            }
            current = current->next;
        }
    }

    if (last_instance != NULL) {
        offset = last_instance->content->offset + var_type_size;
    } else {
        offset = var_type_size;
    }

    return offset;
}