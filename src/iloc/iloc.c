#include "iloc.h"

inst_t *create_inst(op_t op, char *op1, char *op2, char *op3) {
    inst_t *inst = (inst_t *)malloc(sizeof(inst_t));

    inst->op = (char *)op_t_str[op];
    inst->op1 = op1;
    inst->op2 = op2;
    inst->op3 = op3;

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

    // TODO: fix this printing logic for other types of instructions
    printf("%s", inst->op);
    if (inst->op1 != NULL) {
        printf(" %s", inst->op1);
    }
    if (inst->op2 != NULL) {
        printf(", %s", inst->op2);
    }
    if (inst->op3 != NULL) {
        printf(" => %s", inst->op3);
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