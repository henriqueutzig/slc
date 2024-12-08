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

    sprintf(reg, "r%d", r++);
    return reg;
}

char *gen_label() {
    static unsigned int l = 0;
    char *label = (char *)malloc(LABEL_SIZE * sizeof(char));

    sprintf(label, "L%d", l++);
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


// **************** DEBUG **************** 

void print_inst(inst_t *inst) {
    assert(inst != NULL);
    assert(inst->op != NULL);

    switch (inst->inst)
    {
    case NOP:
    case HALT:
        printf("%s", inst->op);
        break;
    case ADD:
    case SUB:
    case MULT:
    case DIV:
    case ADD_I:
    case SUB_I: 
    case R_SUB_I:
    case MULT_I:
    case DIV_I:
    case R_DIV_I:
    case LSHIFT:
    case LSHIFT_I:
    case RSHIFT:
    case RSHIFT_I:
    case AND:   
    case AND_I:
    case OR:
    case OR_I:
    case XOR:
    case XOR_I:
    case LOAD_AI:
    case LOAD_A0:
    case CLOAD_AI:
    case CLOAD_A0:
    case CMP_LT:
    case CMP_LE:
    case CMP_EQ:
    case CMP_GE:
    case CMP_GT:
    case CMP_NE:
        assert(inst->op1 != NULL);
        assert(inst->op2 != NULL);
        assert(inst->op3 != NULL);
        if (inst->label != NULL) 
            printf("%s: ", inst->label);
        printf("%s %s, %s => %s", inst->op, inst->op1, inst->op2, inst->op3);
        break;
    case STORE_AI:
    case STORE_AO:
    case CSTORE_AI:
    case CSTORE_AO:
    case CBR:
        assert(inst->op1 != NULL);
        assert(inst->op2 != NULL);
        assert(inst->op3 != NULL);
        if (inst->label != NULL) 
            printf("%s: ", inst->label);
        printf("%s %s => %s, %s", inst->op, inst->op1, inst->op2, inst->op3);
        break;
    case LOAD_I:
    case LOAD:
    case CLOAD:
    case STORE:
    case CSTORE:
    case I2I:
    case C2C:
    case C2I:
    case I2C:
        assert(inst->op1 != NULL);
        assert(inst->op2 != NULL);
        if (inst->label != NULL) 
            printf("%s: ", inst->label);
        printf("%s %s => %s", inst->op, inst->op1, inst->op2);
        break;
    case JUMP_I:
    case JUMP:
        assert(inst->op1 != NULL);
        if (inst->label != NULL) 
            printf("%s: ", inst->label);
        printf("%s => %s", inst->op, inst->op1);
        break;
    default:
        break;
    }
    printf("\n");
}

void print_inst_block(inst_block_t *block) {
    inst_block_t *current = block;
    while (current != NULL) {
        print_inst(current->inst);
        current = current->next;
    }
}