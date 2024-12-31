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

    sprintf(label, "l%d", l++);
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

void print_inst(inst_t *inst) {
    assert(inst != NULL);
    assert(inst->op != NULL);

    if (inst->label != NULL) {
        printf("%s:\n", inst->label);
    }

    switch (inst->inst)
    {
    case NOP:
        printf("    nop\n");
        break;
    case ADD:
        printf("    addl %s, %s\n", inst->op1, inst->op2);
        printf("    movl %s, %s\n", inst->op2, inst->op3);
        break;
    case SUB:
        printf("    subl %s, %s\n", inst->op1, inst->op2);
        printf("    movl %s, %s\n", inst->op2, inst->op3);
        break;
    case MULT:
        printf("    imull %s, %s\n", inst->op1, inst->op2);
        printf("    movl %s, %s\n", inst->op2, inst->op3);
        break;
    case DIV:
        printf("    movl %s, %%eax\n", inst->op1);
        printf("    cltd\n");
        printf("    idivl %s\n", inst->op2);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
    case ADD_I:
        printf("    addl $%s, %s\n", inst->op1, inst->op2);
        break;
    case SUB_I:
        printf("    subl $%s, %s\n", inst->op1, inst->op2);
        break;
    case MULT_I:
        printf("    imull $%s, %s\n", inst->op2, inst->op1);
        break;
    case DIV_I:
        printf("    movl %s, %%eax\n", inst->op1);
        printf("    cltd\n");
        printf("    idivl $%s\n", inst->op2);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
    case LSHIFT:
        printf("    shll %s, %s\n", inst->op2, inst->op1);
        printf("    movl %s, %s\n", inst->op1, inst->op3);
        break;
    case LSHIFT_I:
        printf("    shll $%s, %s\n", inst->op2, inst->op1);
        break;
    case RSHIFT:
        printf("    shrl %s, %s\n", inst->op2, inst->op1);
        printf("    movl %s, %s\n", inst->op1, inst->op3);
        break;
    case RSHIFT_I:
        printf("    shrl $%s, %s\n", inst->op2, inst->op1);
        break;
    case AND:
        printf("    andl %s, %s\n", inst->op1, inst->op2);
        printf("    movl %s, %s\n", inst->op2, inst->op3);
        break;
    case AND_I:
        printf("    andl $%s, %s\n", inst->op1, inst->op2);
        break;
    case OR:
        printf("    orl %s, %s\n", inst->op1, inst->op2);
        printf("    movl %s, %s\n", inst->op2, inst->op3);
        break;
    case OR_I:
        printf("    orl $%s, %s\n", inst->op1, inst->op2);
        break;
    case XOR:
        printf("    xorl %s, %s\n", inst->op1, inst->op2);
        printf("    movl %s, %s\n", inst->op2, inst->op3);
        break;
    case XOR_I:
        printf("    xorl $%s, %s\n", inst->op1, inst->op2);
        break;
    case LOAD_I:
        printf("    movl $%s, %s\n", inst->op1, inst->op2);
        break;
    case LOAD:
        printf("    movl (%s), %s\n", inst->op1, inst->op2);
        break;
    case LOAD_AI:
        printf("    movl %s(%%rip), %s\n", inst->op1, inst->op3);
        break;
    case LOAD_A0:
        printf("    movl %s, %s\n", inst->op1, inst->op3);
        break;
    case CLOAD:
        printf("    movzbl (%s), %s\n", inst->op1, inst->op2);
        break;
    case CLOAD_AI:
        printf("    movzbl %s(%%rip), %s\n", inst->op1, inst->op3);
        break;
    case CLOAD_A0:
        printf("    movzbl %s, %s\n", inst->op1, inst->op3);
        break;
    case STORE:
        printf("    movl %s, (%s)\n", inst->op1, inst->op2);
        break;
    case STORE_AI:
        printf("    movl %s, %s(%%rip)\n", inst->op1, inst->op2);
        break;
    case STORE_AO:
        printf("    movl %s, %s(%s)\n", inst->op1, inst->op2, inst->op3);
        break;
    case CSTORE:
        printf("    movb %s, (%s)\n", inst->op1, inst->op2);
        break;
    case CSTORE_AI:
        printf("    movb %s, %s(%%rip)\n", inst->op1, inst->op2);
        break;
    case CSTORE_AO:
        printf("    movb %s, %s(%s)\n", inst->op1, inst->op2, inst->op3);
        break;
    case I2I:
        printf("    movl %s, %s\n", inst->op1, inst->op2);
        break;
    case C2C:
        printf("    movb %s, %s\n", inst->op1, inst->op2);
        break;
    case C2I:
        printf("    movzbl %s, %s\n", inst->op1, inst->op2);
        break;
    case I2C:
        printf("    movb %s, %s\n", inst->op1, inst->op2);
        break;
    case JUMP_I:
        printf("    jmp %s\n", inst->op1);
        break;
    case JUMP:
        printf("    jmp *%s\n", inst->op1);
        break;
    case CBR:
        printf("    cmpl $0, %s\n", inst->op1);
        printf("    je %s\n", inst->op2);
        printf("    jmp %s\n", inst->op3);
        break;
    case CMP_LT:
        printf("    cmpl %s, %s\n", inst->op1, inst->op2);
        printf("    setl %%al\n");
        printf("    movzbl %%al, %s\n", inst->op3);
        break;
    case CMP_LE:
        printf("    cmpl %s, %s\n", inst->op1, inst->op2);
        printf("    setle %%al\n");
        printf("    movzbl %%al, %s\n", inst->op3);
        break;
    case CMP_EQ:
        printf("    cmpl %s, %s\n", inst->op1, inst->op2);
        printf("    sete %%al\n");
        printf("    movzbl %%al, %s\n", inst->op3);
        break;
    case CMP_GE:
        printf("    cmpl %s, %s\n", inst->op1, inst->op2);
        printf("    setge %%al\n");
        printf("    movzbl %%al, %s\n", inst->op3);
        break;
    case CMP_GT:
        printf("    cmpl %s, %s\n", inst->op1, inst->op2);
        printf("    setg %%al\n");
        printf("    movzbl %%al, %s\n", inst->op3);
        break;
    case CMP_NE:
        printf("    cmpl %s, %s\n", inst->op1, inst->op2);
        printf("    setne %%al\n");
        printf("    movzbl %%al, %s\n", inst->op3);
        break;
    default:
        printf("    # Unsupported operation , operation is %s\n", inst->op);
        break;
    }
}


void print_inst_block(inst_block_t *block) {
    inst_block_t *current = block;
    while (current != NULL) {
        print_inst(current->inst);
        current = current->next;
    }
}
