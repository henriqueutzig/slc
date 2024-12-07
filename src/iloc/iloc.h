#ifndef __ILOC_H__
#define __ILOC_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#define REG_SIZE 10
#define LABEL_SIZE 10

typedef enum {
    NOP,
    HALT,
    ADD,
    SUB,
    MULT,
    DIV,
    ADD_I,
    SUB_I,
    R_SUB_I,
    MULT_I,
    DIV_I,
    R_DIV_I,
    LSHIFT,
    LSHIFT_I,
    RSHIFT,
    RSHIFT_I,
    AND,
    AND_I,
    OR,
    OR_I,
    XOR,
    XOR_I,
    LOAD_I,
    LOAD,
    LOAD_AI,
    LOAD_A0,
    CLOAD,
    CLOAD_AI,
    CLOAD_A0,
    STORE,
    STORE_AI,
    STORE_AO,
    CSTORE,
    CSTORE_AI,
    CSTORE_AO,
    I2I,
    C2C,
    C2I,
    I2C,
    JUMP_I,
    JUMP,
    CBR,
    CMP_LT,
    CMP_LE,
    CMP_EQ,
    CMP_GE,
    CMP_GT,
    CMP_NE
} op_t;

static const char *op_t_str[] = {
    "nop",
    "add",
    "sub",
    "mult",
    "div",
    "addI",
    "subI",
    "rsubI",
    "multI",
    "divI",
    "rdivI",
    "lshift",
    "lshiftI",
    "rshift",
    "rshiftI",
    "and",
    "andI",
    "or",
    "orI",
    "xor",
    "xorI",
    "loadI",
    "load",
    "loadAI",
    "loadA0",
    "cload",
    "cloadAI",
    "cloadA0",
    "store",
    "storeAI",
    "storeAO",
    "cstore",
    "cstoreAI",
    "cstoreAO",
    "i2i",
    "c2c",
    "c2i",
    "i2c",
    "jumpI",
    "jump",
    "cbr",
    "cmp_LT",
    "cmp_LE",
    "cmp_EQ",
    "cmp_GE",
    "cmp_GT",
    "cmp_NE"
};

typedef struct inst_t
{
    char* op; 
    char *op1;
    char *op2;
    char *op3;

    char *label;
    op_t inst;
} inst_t;

typedef struct inst_block_t 
{
    inst_t *inst;
    struct inst_block_t *next;
} inst_block_t;

inst_t *create_inst(op_t op, char *op1, char *op2, char *op3, char *label);
inst_block_t *create_inst_block(inst_t *inst, ...);
inst_block_t *append_inst(inst_block_t *block, inst_t *inst);
inst_block_t *append_inst_block(inst_block_t *block, inst_block_t *block2);

void destroy_inst(inst_t *inst);
void destroy_inst_block(inst_block_t *block);

void print_inst(inst_t *inst);
void print_inst_block(inst_block_t *block);

char *gen_reg();
char *gen_label();
#endif