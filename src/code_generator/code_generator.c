/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "code_generator.h"

char *parse_unsigned_int(unsigned int value) {
    char *str = (char *)malloc(12);
    sprintf(str, "%d", value);
    return str;
}

inst_block_t* generate_load_literal(char *valor, char *temp) {
    inst_t *inst = create_inst(LOAD_I, valor, temp, NULL, NULL);
    inst_block_t *block = create_inst_block(inst, NULL);
    return block;
}

inst_block_t* generate_atribuicao(asd_tree_t *target, asd_tree_t *expr,unsigned int target_offset) {
    inst_t *inst = create_inst(STORE_AI, expr->temp, "rfp",parse_unsigned_int(target_offset) ,NULL);
    inst_block_t *block = create_inst_block(inst, NULL);    
    block = append_inst_block(expr->code,block);

    return block;
}

inst_block_t* generate_load(lexema *literal, char *temp) {
    inst_t *inst = create_inst(LOAD_I, literal->valor, temp, NULL, NULL);
    inst_block_t *block = create_inst_block(inst, NULL);
    return block;
}

inst_block_t* generate_expression(char *target_temp, char *op1, char *op2,op_t operation){
    inst_t *inst = create_inst(operation, op1, op2, target_temp, NULL);
    inst_block_t *block = create_inst_block(inst, NULL);
    print_inst_block(block);
    return block;
}

inst_block_t* generate_load_ident(asd_tree_t *base, char *temp,unsigned int target_offset){
    inst_t *inst = create_inst(LOAD_AI, "rfp",parse_unsigned_int(target_offset),temp ,NULL);
    inst_block_t *block = create_inst_block(inst, NULL);
    return block;
}

int is_integer(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    char *p;
    strtol(str, &p, 10);
    bool is_int = *p == '\0';
    return is_int;
}


inst_block_t *generate_load_inner(asd_tree_t *node, char *temp, stackt_t *stack) {
    if (is_integer(node->label)) {
        return generate_load_literal(node->label, temp);
    } else {
        return generate_load_ident(node, temp, get_offset_from_stack(stack, node->label));
    }
}





void generate_expression_code(asd_tree_t* target, asd_tree_t *op1, asd_tree_t *op2, op_t operation,stackt_t *stack){
        char *temp1 = gen_reg();
        char *temp2 = gen_reg();


        inst_block_t *bloco_1 = generate_load_inner(op1, temp1, stack);
        inst_block_t *bloco_2 = generate_load_inner(op2, temp2, stack);
        
        bloco_2 = append_inst_block(bloco_1, bloco_2);

        target->temp = gen_reg();
        target->code = generate_expression(target->temp, temp1, temp2, operation);
        target->code = append_inst_block(bloco_2, target->code);

}

