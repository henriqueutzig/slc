#include "iloc.h"

inst_t *create_inst(op_t op, char *op1, char *op2, char *op3) {
    inst_t *inst = (inst_t *)malloc(sizeof(inst_t));

    inst->op = op_t_str[op];
    inst->op1 = op1;
    inst->op2 = op2;
    inst->op3 = op3;

    return inst;
}