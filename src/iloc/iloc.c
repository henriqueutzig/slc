/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "iloc.h"
#define REG_SIZE 10

char *gen_reg() {
    static unsigned int r = 0;
    char *reg = (char *)malloc(REG_SIZE * sizeof(char));


    switch (r) {
        case 0: snprintf(reg, REG_SIZE, "%%ebx"); break;
        case 1: snprintf(reg, REG_SIZE, "%%ecx"); break;
        case 2: snprintf(reg, REG_SIZE, "%%edx"); break;
        case 3: snprintf(reg, REG_SIZE, "%%esi"); break;
        case 4: snprintf(reg, REG_SIZE, "%%edi"); break;
        case 5: snprintf(reg, REG_SIZE, "%%r8d"); break;
        case 6: snprintf(reg, REG_SIZE, "%%r9d"); break;
        case 7: snprintf(reg, REG_SIZE, "%%r10d"); break;
        case 8: snprintf(reg, REG_SIZE, "%%r11d"); break;
        default: snprintf(reg, REG_SIZE, "r%u", r - 10); break;
    }

    r++;
    return reg;
}


char *gen_label() {
    static unsigned int l = 0;
    char *label = (char *)malloc(LABEL_SIZE * sizeof(char));

    sprintf(label, ".L%d", l++);
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
        printf("    # No operation\n");
        break;
    case ADD:
        printf("    movl %s, %%eax\n", inst->op1);
        printf("    addl %s, %%eax\n", inst->op2);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
    case SUB:
        printf("    movl %s, %%eax\n", inst->op1);
        printf("    subl %s, %%eax\n", inst->op2);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
    case MULT:
        printf("    movl %s, %%eax\n", inst->op1);
        printf("    imull %s, %%eax\n", inst->op2);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
    case DIV:
        printf("    movl %s, %%eax\n", inst->op1);
        printf("    cltd\n");
        printf("    idivl %s\n", inst->op2);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
    case ADD_I:
        printf("    movl %s, %%eax\n", inst->op2);
        printf("    addl $%s, %%eax\n", inst->op1);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
    case SUB_I:
        printf("    movl %s, %%eax\n", inst->op2);
        printf("    subl $%s, %%eax\n", inst->op1);
        printf("    movl %%eax, %s\n", inst->op3);
        break;
   case LOAD_I:
        printf("    movl $%s, %s\n", inst->op1,inst->op2);
        break;
   case STORE_AI:
        printf("    movl %s, -%s(%%rbp)\n",inst->op1,inst->op3);
        break;
    case JUMP:
        printf("    jmp %s\n", inst->op1);
        break;
    case LOAD_AI:
        printf("    movl -%s(%%rbp), %s\n",inst->op2,inst->op3);
        break;
    case RET:
        printf("    movl $0, %%eax\n");
        printf("    leave\n");
        printf("    ret\n");
        break;
    default:
        printf("    # Unsupported operation: %s\n", inst->op);
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
