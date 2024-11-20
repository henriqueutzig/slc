#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../symbol_table/symbol_table.h"

#define STACK_SIZE 100

typedef struct stack_t {
    symbol_table_t **tables[STACK_SIZE];
    int top_index;
} stack_t;

stack_t *create_stack();
bool is_empty(stack_t *stack);
bool is_full(stack_t *stack);
void destroy_stack(stack_t *stack);
stack_t *push_symbol_table(stack_t *stack, symbol_table_t *table);
symbol_table_t **pop_symbol_table(stack_t *stack);

#endif