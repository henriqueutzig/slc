/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "iloc.h"

char *gen_reg() {
    static unsigned int r = 0;
    char *reg = (char *)malloc(REG_SIZE * sizeof(char));

    sprintf(reg, "r%%d", r++);
    return reg;
}

char *gen_label() {
    static unsigned int l = 0;
    char *label = (char *)malloc(LABEL_SIZE * sizeof(char));

    sprintf(label, "l%%d", l++);
    return label;
}

inst_t *create_inst(op_t op, char *op1, char *op2, char *op3, char *label) {
    inst_t *inst = (inst_t *)malloc(sizeof(inst_t));

    inst->op = (char *)op_t_str[op];
    inst->op1 = op1;
    inst->op2 = op2;
    inst->op3 = op3;
    inst->label = label;

    inst->inst = op;

    return inst;
}

inst_block_t *create_inst_block(inst_t *inst, ...) {
    va_list args;
    va_start(args, inst);

    inst_block_t *block = (inst_block_t *)malloc(sizeof(inst_block_t));
    block->inst = inst;
    block->next = NULL;

    inst_block_t *current = block;
    inst_t *next_inst = va_arg(args, inst_t *);
    while (next_inst != NULL) {
        inst_block_t *new_block = (inst_block_t *)malloc(sizeof(inst_block_t));
        new_block->inst = next_inst;
        new_block->next = NULL;
        current->next = new_block;
        current = new_block;
        next_inst = va_arg(args, inst_t *);
    }

    va_end(args);
    return block;
}

inst_block_t *append_inst(inst_block_t *block, inst_t *inst) {
    inst_block_t *current = block;
    while (current->next != NULL) {
        current = current->next;
    }

    inst_block_t *new_block = (inst_block_t *)malloc(sizeof(inst_block_t));
    new_block->inst = inst;
    new_block->next = NULL;
    current->next = new_block;

    return block;
}

inst_block_t *append_inst_block(inst_block_t *block, inst_block_t *block2) {
    inst_block_t *current = block;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = block2;
    return block;
}

void destroy_inst(inst_t *inst) {
    free(inst);
}

void destroy_inst_block(inst_block_t *block) {
    inst_block_t *current = block;
    while (current != NULL) {
        inst_block_t *next = current->next;
        destroy_inst(current->inst);
        free(current);
        current = next;
    }
}

void print_inst_block(inst_block_t *block){
    inst_t *inst = block->inst;
    assert(inst != NULL);
    assert(inst->op != NULL);

    if (inst->label != NULL) {
        printf("%s:\n", inst->label);
    }

    switch (inst->inst) {
    case LOAD_I:
        printf("\tmov $%s, %s\n", inst->op1, inst->op2);
        break;
    case ADD:
        printf("\tadd %s, %s\n", inst->op1, inst->op2);
        printf("\tmov %s, %s\n", inst->op2, inst->op3);
        break;
    case SUB:
        printf("\tsub %s, %s\n", inst->op1, inst->op2);
        printf("\tmov %s, %s\n", inst->op2, inst->op3);
        break;
    case MULT:
        printf("\timul %s, %s\n", inst->op1, inst->op2);
        printf("\tmov %s, %s\n", inst->op2, inst->op3);
        break;
    case DIV:
        printf("\tmov %s, %%rax\n", inst->op1);
        printf("\tcqo\n");
        printf("\tidiv %s\n", inst->op2);
        printf("\tmov %%rax, %s\n", inst->op3);
        break;
    case JUMP_I:
        printf("\tjmp %s\n", inst->op1);
        break;
    case CBR:
        printf("\tcmp $0, %s\n", inst->op1);
        printf("\tje %s\n", inst->op2);
        printf("\tjmp %s\n", inst->op3);
        break;
    default:
        printf("\t# Unsupported operation: %s\n", inst->op);
        break;
    }
}
