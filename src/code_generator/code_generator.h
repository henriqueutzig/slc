/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/
#ifndef __CODE_GENERATOR_H__
#define __CODE_GENERATOR_H__

#include <lexema.h>
#include <iloc.h>
#include <asd.h>
#include <stack.h>

inst_block_t* generate_load_literal(char *valor, char *temp);
inst_block_t* generate_atribuicao(asd_tree_t *target, asd_tree_t *expr,unsigned int target_offset);
inst_block_t* generate_expression(char *target_temp, char *op1, char *op2,op_t operation);
inst_block_t* generate_load_ident(asd_tree_t *base, char *temp,unsigned int target_offset);
void generate_expression_code(asd_tree_t* target, asd_tree_t *op1, asd_tree_t *op2, op_t operation,stackt_t *stack);
void generate_not(asd_tree_t* target, asd_tree_t *op1,stackt_t *stack);
void generate_neg(asd_tree_t* target, asd_tree_t *op1,stackt_t *stack);
void generate_if(asd_tree_t* target, asd_tree_t *boolean_op, asd_tree_t *body, stackt_t *stack);




#endif