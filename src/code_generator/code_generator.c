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

inst_block_t* generate_load_literal(lexema *literal, char *temp) {
    inst_t *inst = create_inst(LOAD_I, literal->valor, temp, NULL, NULL);
    inst_block_t *block = create_inst_block(inst, NULL);
    return block;
}

inst_block_t* generate_atribuicao(asd_tree_t *target, asd_tree_t *expr,unsigned int target_offset,char *temp) {

    inst_t *inst = create_inst(STORE_AI, expr->temp, target->temp,parse_unsigned_int(target_offset) ,NULL);
    inst_block_t *block = create_inst_block(inst, NULL);
    block = append_inst_block(expr->code,block);

    return block;
}

