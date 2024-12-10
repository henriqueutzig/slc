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
        if(node->code == NULL){
            return generate_load_ident(node, temp, get_offset_from_stack(stack, node->label));
        }
        node->temp = temp;
        return node->code;
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

void generate_neg(asd_tree_t* target, asd_tree_t *op1, stackt_t *stack){
        char *temp1 = gen_reg();

        inst_block_t *bloco_1 = generate_load_inner(op1, temp1, stack);

        target->temp = gen_reg();

        inst_t *inst = create_inst(MULT_I, temp1, "-1", target->temp, NULL);
        inst_block_t *bloco_2 = create_inst_block(inst, NULL);
        bloco_2 = append_inst_block(bloco_1, bloco_2);

        target->code = bloco_2;
}

void generate_not(asd_tree_t* target, asd_tree_t *op1,stackt_t *stack){
        char *temp1 = gen_reg();
        char *temp3 = gen_reg();
        char *temp4 = gen_reg();

        char *label1 = gen_label();
        char *label2 = gen_label();

        inst_block_t *bloco_principal = generate_load_inner(op1, temp1, stack);
        inst_block_t *if_load_zero_for_comp = generate_load_literal("0", temp4);
        
        inst_t *cmp_with_zero = create_inst(CMP_EQ, temp1, temp4, temp3, NULL);
        inst_block_t *bloco_cmp_with_zero = create_inst_block(cmp_with_zero, NULL);

        inst_t *inst = create_inst(CBR, temp3, label1, label2, NULL);
        inst_block_t *bloco_jump_condicional = create_inst_block(inst, NULL);

        inst = create_inst(LOAD_I, "1", temp1, NULL, label1);
        inst_block_t *bloco_salva_1 = create_inst_block(inst);

        inst = create_inst(LOAD_I, "0", temp1, NULL, label2);
        inst_block_t *bloco_salva_0 = create_inst_block(inst);

        if_load_zero_for_comp = append_inst_block(bloco_principal, if_load_zero_for_comp);
        bloco_cmp_with_zero = append_inst_block(if_load_zero_for_comp, bloco_cmp_with_zero);
        bloco_cmp_with_zero = append_inst_block(bloco_cmp_with_zero, bloco_jump_condicional);
        bloco_salva_0 = append_inst_block(bloco_salva_1, bloco_salva_0);
        bloco_cmp_with_zero = append_inst_block(bloco_cmp_with_zero, bloco_salva_0);

        target->temp = gen_reg();
        target->code = bloco_cmp_with_zero;

}


void generate_if(asd_tree_t* target, asd_tree_t *boolean_op, asd_tree_t *body, stackt_t *stack){
        char *temp3 = gen_reg();
        char *temp4 = gen_reg();

        char *label1 = gen_label();
        char *label2 = gen_label();

        body->code->inst->label = label1;

        inst_block_t *if_load_zero_for_comp = generate_load_literal("0", temp4);
        inst_t *inst = create_inst(CMP_EQ, boolean_op->temp, temp4,temp3, NULL);
        inst_block_t *bloco_if = create_inst_block(inst);
        bloco_if = append_inst_block(if_load_zero_for_comp, bloco_if);
        bloco_if = append_inst_block(boolean_op->code, bloco_if);

        inst = create_inst(CBR, temp3, label2, label1, NULL);
        inst_block_t *bloco_jump_condicional = create_inst_block(inst);
        
        inst = create_inst(NOP, NULL, NULL, NULL, label2);
        inst_block_t *bloco_proxima_instr = create_inst_block(inst);

        bloco_if = append_inst_block(bloco_if, bloco_jump_condicional);
        bloco_if = append_inst_block(bloco_if, body->code);
        bloco_if = append_inst_block(bloco_if, bloco_proxima_instr);

        target->temp = gen_reg();
        target->code = bloco_if;
}

void generate_if_with_else(asd_tree_t* target, asd_tree_t *boolean_op, asd_tree_t *body, asd_tree_t *else_body, stackt_t *stack){
        char *temp3 = gen_reg();
        char *temp4 = gen_reg();

        char *label1 = gen_label();
        char *label2 = gen_label();
        char *label3 = gen_label();

        body->code->inst->label = label1;
        else_body->code->inst->label = label2;

        inst_block_t *if_load_zero_for_comp = generate_load_literal("0", temp4);
        inst_t *inst = create_inst(CMP_EQ, boolean_op->temp, temp4,temp3, NULL);
        inst_block_t *bloco_if = create_inst_block(inst);
        bloco_if = append_inst_block(if_load_zero_for_comp, bloco_if);
        bloco_if = append_inst_block(boolean_op->code, bloco_if);

        inst = create_inst(CBR, temp3, label3, label1, NULL);
        inst_block_t *bloco_jump_condicional = create_inst_block(inst);
        
        inst = create_inst(NOP, NULL, NULL, NULL, label3);
        inst_block_t *bloco_proxima_instr = create_inst_block(inst);

        inst = create_inst(JUMP_I, label3, NULL, NULL, NULL);
        inst_block_t *bloco_jump_sobre_else = create_inst_block(inst);
        bloco_if = append_inst_block(bloco_if, bloco_jump_sobre_else);

        bloco_if = append_inst_block(bloco_if, bloco_jump_condicional);
        bloco_if = append_inst_block(bloco_if, body->code);
        bloco_if = append_inst_block(bloco_if, else_body->code);
        bloco_if = append_inst_block(bloco_if, bloco_proxima_instr);

        target->temp = gen_reg();
        target->code = bloco_if;
}

void generate_while(asd_tree_t* target, asd_tree_t *boolean_op, asd_tree_t *body, stackt_t *stack){
        char *temp3 = gen_reg();
        char *temp4 = gen_reg();

        char *label1 = gen_label();
        char *label2 = gen_label();
        char *label3 = gen_label();

        body->code->inst->label = label1;

        inst_block_t *if_load_zero_for_comp = generate_load_literal("0", temp4);
        inst_t *inst = create_inst(CMP_EQ, boolean_op->temp, temp4,temp3, label3);
        inst_block_t *bloco_if = create_inst_block(inst);
        bloco_if = append_inst_block(if_load_zero_for_comp, bloco_if);
        bloco_if = append_inst_block(boolean_op->code, bloco_if);

        inst = create_inst(CBR, temp3, label2, label1, NULL);
        inst_block_t *bloco_jump_condicional = create_inst_block(inst);
        
        inst = create_inst(NOP, NULL, NULL, NULL, label2);
        inst_block_t *bloco_proxima_instr = create_inst_block(inst);

        inst = create_inst(JUMP_I, label3, NULL, NULL, NULL);
        inst_block_t *bloco_jump_sobre_else = create_inst_block(inst);
        bloco_proxima_instr = append_inst_block(bloco_jump_sobre_else, bloco_proxima_instr);

        bloco_if = append_inst_block(bloco_if, bloco_jump_condicional);
        bloco_if = append_inst_block(bloco_if, body->code);
        bloco_if = append_inst_block(bloco_if, bloco_proxima_instr);

        target->temp = gen_reg();
        target->code = bloco_if;
}