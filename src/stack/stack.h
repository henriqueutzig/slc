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

typedef struct stack_tt {
    symbol_table_t **tables[STACK_SIZE];
    int top_index;
} stack_tt;

stack_tt *create_stack();
bool is_empty(stack_tt *stack);
bool is_full(stack_tt *stack);
void destroy_stack(stack_tt *stack);
stack_tt *push_symbol_table(stack_tt *stack, symbol_table_t *table);
symbol_table_t *pop_symbol_table(stack_tt *stack);

#endif