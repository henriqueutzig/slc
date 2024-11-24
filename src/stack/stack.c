/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "stack.h"

stackt_t *create_stack()
{
    stackt_t *stack = (stackt_t *)malloc(sizeof(stackt_t));

    for (int i = 0; i < STACK_SIZE; i++)
       stack->tables[i] = NULL;

    stack->top_index = -1;
    return stack;
}

bool is_empty(stackt_t *stack)
{
    return stack->top_index == -1;
}

bool is_full(stackt_t *stack)
{
    return stack->top_index == STACK_SIZE - 1;
}

void destroy_stack(stackt_t *stack)
{
    for (int i = 0; i < STACK_SIZE; i++)
        if (stack->tables[i] != NULL)
            destroy_symbol_table(*stack->tables[i]);

    free(stack);
}

stackt_t *push_symbol_table(stackt_t *stack, symbol_table_t *table) {
    if (is_full(stack)) {
        printf("Stack is full\n");
        return stack;
    }

    symbol_table_t **table_ptr = (symbol_table_t **)malloc(sizeof(symbol_table_t *));
    *table_ptr = table; 

    stack->top_index++;
    stack->tables[stack->top_index] = table_ptr;

    return stack;
}

symbol_table_t *pop_symbol_table(stackt_t *stack)
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


void insert_symbol_to_global_scope(stackt_t *stack, lexema *lexema, int line, type_t type){
    if (is_empty(stack)) {
        printf("ERROR: stack is empty!");
        return;
    }

    content_t *symbol = search_all_tables(stack, lexema->valor);
    if (symbol != NULL) {
        printf("Erro na linha %d: símbolo já declarado. O símbolo %s foi previamente declarado na linha %d.\n", line, lexema->valor, symbol->line);
        exit(ERR_DECLARED);
    }

    content_t *content = create_content(line, lexema, type);
    insert_element(*stack->tables[0], lexema->valor, content);
}

void insert_symbol_to_scope(stackt_t *stack, lexema *lexema, int line, type_t type){
    if (is_empty(stack)) {
        printf("ERROR: stack is empty!");
        return;
    }

    content_t *symbol = search_all_tables(stack, lexema->valor);
    if (symbol != NULL) {
        printf("Erro na linha %d: símbolo já declarado. O símbolo %s foi previamente declarado na linha %d.\n", line, lexema->valor, symbol->line);
        exit(ERR_DECLARED);
    }

    content_t *content = create_content(line, lexema, type);
    insert_element(*stack->tables[stack->top_index], lexema->valor, content);
}

content_t *search_all_tables(stackt_t *stack, char *lexema) {
    for (int i = stack->top_index; i >= 0; i--) {
        symbol_table_t *table = *(stack->tables[i]); 
        content_t *result = search_table(table, lexema); 
        if (result != NULL) {
            return result; 
        }
    }
    return NULL; 
}