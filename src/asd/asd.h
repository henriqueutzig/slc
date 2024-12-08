#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "content.h"
#include "iloc.h"

typedef struct asd_tree {
  char *label;
  int number_of_children;
  struct asd_tree **children;
  type_t type;

  inst_block_t *code; 
  char *temp;
} asd_tree_t;

type_t infer_node_type(asd_tree_t *first_child, asd_tree_t *second_child);
asd_tree_t *asd_new_typed(const char *label, type_t type);

/*
 * Função asd_new, cria um nó sem filhos com o label informado.
 */
asd_tree_t *asd_new(const char *label);

/*
 * Função asd_tree, libera recursivamente o nó e seus filhos.
 */
void asd_free(asd_tree_t *tree);

/*
 * Função asd_add_child, adiciona child como filho de tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Função asd_print, imprime recursivamente a árvore.
 */
void asd_print(asd_tree_t *tree);

/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz (asd_tree_t *tree);

/*
 * Função asd_last_child, retorna o último filho da árvore.
 */
asd_tree_t *asd_last_child(asd_tree_t *tree);


#endif //_ARVORE_H_
