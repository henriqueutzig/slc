/*
    #####################################
    # 		Authors - Grupo J:			#
    # 	Henrique Utzig - 00319043		#
    # 	João Pedro Cosme - 00314792		#
    #####################################
*/

#include "code_generator.h"

inst_block_t* generate_load_literal(lexema *literal, char *reg) {
    inst_t *inst = create_inst(LOAD_I, literal->valor, NULL, reg, NULL);
    inst_block_t *block = create_inst_block(inst, NULL);
    return block;
}