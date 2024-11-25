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
    // fprintf(stderr, "Pushing table %p to stack %p\n", table, stack);
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
symbol_table_t *pop_symbol_table(stackt_t *stack) {
    if (is_empty(stack)) {
        return NULL;
    }

    symbol_table_t **table_ptr = stack->tables[stack->top_index];
    symbol_table_t *table = *table_ptr;

    free(table_ptr); 
    stack->tables[stack->top_index] = NULL;
    stack->top_index--;
    return table;
}


void insert_symbol_to_global_scope(stackt_t *stack, lexema *lexema, int line, type_t type){
    if (is_empty(stack)) {
        printf("ERROR (insert_symbol_to_global_scope): stack is empty!\n");
        return;
    }

    content_t *symbol = search_all_tables(stack, lexema->valor);
    if (symbol != NULL) {
        printf("Erro na linha %d: símbolo já declarado. O símbolo %s foi previamente declarado na linha %d.\n", line, lexema->valor, symbol->line);
        exit(ERR_DECLARED);
    }

    content_t *content = create_content(line, lexema, type, FUNCTION);
    insert_element(*stack->tables[0], lexema->valor, content);
}

void insert_symbol_to_scope(stackt_t *stack, lexema *lexema, int line, type_t type){
    if (is_empty(stack)) {
        printf("ERROR (insert_symbol_to_scope): stack is empty! Could not insert symbol '%s'\n", lexema->valor);
        return;
    }

    content_t *symbol = search_all_tables(stack, lexema->valor);
    if (symbol != NULL) {
        printf("Erro na linha %d: símbolo já declarado. O símbolo %s foi previamente declarado na linha %d.\n", line, lexema->valor, symbol->line);
        exit(ERR_DECLARED);
    }

    content_t *content = create_content(line, lexema, type, VARIABLE);
    insert_element(*stack->tables[stack->top_index], lexema->valor, content);
}

void validate_attribution(stackt_t *stack, lexema *lexema, type_t type, int line){
    if (is_empty(stack)) {
        printf("ERROR (validate_attribution): stack is empty!\n");
        return;
    }

    content_t *symbol = search_all_tables(stack, lexema->valor);

    if (symbol->type != type) {
        printf("Erro na linha %d: atribuição inválida. O símbolo %s foi declarado como %s e está sendo atribuído como %s na linha %d.\n", symbol->line, lexema->valor, get_type_name(symbol->type), get_type_name(type), line);
        exit(ERR_VARIABLE);
        //Validar se erro correto!
    }
}

void validate_function_call(stackt_t *stack, lexema *lexema, int line){
    bool found_as_variable = false;
    bool found_as_function = false;
    for (int i = stack->top_index; i >= 0; i--) {
        symbol_table_t *table = *(stack->tables[i]); 
        // fprintf(stderr, "Searching lexema %s in table %d, with pointer %p\n",lexema->valor ,i, table);
        content_t *result = search_table(table, lexema->valor); 
        if (result != NULL) {
            fprintf(stderr, "Found lexema %s in table %d\n",lexema->valor ,i);
            if (i!=0){
                found_as_variable = true; 
            }else{
                found_as_function = true;
            }
        }
    }
    if(found_as_function){
        // Tudo certo
    }else if(found_as_variable){
        printf("Erro na linha %d: variavel %s foi usada como funçao!\n", line, lexema->valor);
        exit(ERR_VARIABLE);
    }else{
        printf("Erro na linha %d: símbolo não declarado. O símbolo %s não foi declarado.\n", line, lexema->valor);
        exit(ERR_UNDECLARED);
    }
}

/// DEBUG FUNC
void print_table(symbol_table_t *table){
    printf("-----+-----+-----+-----+-----+\n");
    printf("Index\tLexema\tLine\tType\tNature\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        symbol_table_t *current = &table[i];
        while (current != NULL) {
            if (current->content != NULL) {
                printf("%d\t%s\t%d\t%d\t%d\n", i, current->content_lexema_value, current->content->line, current->content->type, current->content->nature);
            }
            current = current->next;
        }
    }
}

void validate_variable_use(stackt_t *stack, lexema *lexema, int line){
    bool found_as_variable = false;
    bool found_as_function = false;
    for (int i = stack->top_index; i >= 0; i--) {
        symbol_table_t *table = *(stack->tables[i]); 
        print_table(table);
        // fprintf(stderr, "Searching lexema %s in table %d, with pointer %p\n",lexema->valor ,i, table);
        content_t *result = search_table(table, lexema->valor); 
        if (result != NULL) {
            // fprintf(stderr, "Found lexema %s in table %d\n",lexema->valor ,i);
            if (i > 0){
                found_as_variable = true; 
            }else{
                found_as_function = true;
            }
        }
    }
    if(found_as_function){
        printf("Erro na linha %d: funçao %s foi usada como variavel!\n", line, lexema->valor);
        exit(ERR_FUNCTION);
    }else if(found_as_variable){
    // Tudo certo
    }else{
        printf("Erro na linha %d: símbolo não declarado. O símbolo %s não foi declarado.\n", line, lexema->valor);
        exit(ERR_UNDECLARED);
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