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

char *get_type_name(type_t type){
    switch (type)
    {
    case INT:
        return "int";
    case FLOAT:
        return "float";
    default:
        return "unknown";
    }
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

void validate_attribution(stackt_t *stack, lexema *lexema, type_t type, int line){
    if (is_empty(stack)) {
        printf("ERROR: stack is empty!");
        return;
    }

    content_t *symbol = search_all_tables(stack, lexema->valor);

    if (symbol->type != type) {
        printf("Erro na linha %d: atribuição inválida. O símbolo %s foi declarado como %s e está sendo atribuído como %s na linha %d.\n", symbol->line, lexema->valor, get_type_name(symbol->type), get_type_name(type), line);
        exit(ERR_VARIABLE);
        //Validar se erro correto!
    }
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