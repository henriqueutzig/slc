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

inst_block_t* generate_load_literal(lexema *literal, char *reg);

#endif