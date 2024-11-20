/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "stack.h"

stack_tt *create_stack()
{
    stack_tt *stack = (stack_tt *)malloc(sizeof(stack_tt));

    for (int i = 0; i < STACK_SIZE; i++)
       stack->tables[i] = NULL;

    stack->top_index = -1;
    return stack;
}

bool is_empty(stack_tt *stack)
{
    return stack->top_index == -1;
}

bool is_full(stack_tt *stack)
{
    return stack->top_index == STACK_SIZE - 1;
}

void destroy_stack(stack_tt *stack)
{
    for (int i = 0; i < STACK_SIZE; i++)
        if (stack->tables[i] != NULL)
            destroy_symbol_table(*stack->tables[i]);

    free(stack);
}

stack_tt *push_symbol_table(stack_tt *stack, symbol_table_t *table)
{
    if (is_full(stack))
    {
        printf("Stack is full\n");
        return stack;
    }

    stack->top_index++;
    stack->tables[stack->top_index] = &table;
    return stack;
}

symbol_table_t *pop_symbol_table(stack_tt *stack)
{
    if (is_empty(stack))
    {
        printf("Stack is empty\n");
        return NULL;
    }

    symbol_table_t *table = *stack->tables[stack->top_index];
    stack->tables[stack->top_index] = NULL;
    stack->top_index--;
    return table;
}
