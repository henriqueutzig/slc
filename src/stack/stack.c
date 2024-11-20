#include "stack.h"


stack_t *create_stack()
{
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));

    for (int i = 0; i < STACK_SIZE; i++)
       stack->tables[i] = NULL;

    stack->top_index = -1;
    return stack;
}

bool is_empty(stack_t *stack)
{
    return stack->top_index == -1;
}

bool is_full(stack_t *stack)
{
    return stack->top_index == STACK_SIZE - 1;
}

void destroy_stack(stack_t *stack)
{
    for (int i = 0; i < STACK_SIZE; i++)
        if (stack->tables[i] != NULL)
            destroy_symbol_table(stack->tables[i]);

    free(stack);
}

stack_t *push_symbol_table(stack_t *stack, symbol_table_t *table)
{
    if (is_full(stack))
    {
        printf("Stack is full\n");
        return stack;
    }

    stack->top_index++;
    stack->tables[stack->top_index] = table;
    return stack;
}

symbol_table_t **pop_symbol_table(stack_t *stack)
{
    if (is_empty(stack))
    {
        printf("Stack is empty\n");
        return NULL;
    }

    symbol_table_t *table = stack->tables[stack->top_index];
    stack->tables[stack->top_index] = NULL;
    stack->top_index--;
    return table;
}
