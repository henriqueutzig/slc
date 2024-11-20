/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../symbol_table/symbol_table.h"

#define STACK_SIZE 100

typedef struct stackt_t {
    symbol_table_t **tables[STACK_SIZE];
    int top_index;
} stackt_t;

stackt_t *create_stack();
bool is_empty(stackt_t *stack);
bool is_full(stackt_t *stack);
void destroy_stack(stackt_t *stack);
stackt_t *push_symbol_table(stackt_t *stack, symbol_table_t *table);
symbol_table_t *pop_symbol_table(stackt_t *stack);

#endif