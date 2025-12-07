#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include <stdio.h>
#include "Lista.h"
#include "Types.h"

// Estrutura do nó da Árvore Binária Ordenada (ABO)
typedef struct TreeNode {
    long long cpf;      // Chave: CPF
    ListNode* dataNode; // Ponteiro para o nó na lista (LDE) com os dados completos
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Protótipos
TreeNode* create_tree_node(long long cpf, ListNode* dataNode);
TreeNode* insert_abo(TreeNode* root, long long cpf, ListNode* dataNode);

// Funções de análise
int count_nodes(TreeNode* root);
int tree_height(TreeNode* root);
TreeNode* get_min_node(TreeNode* root);
TreeNode* get_max_node(TreeNode* root);

void free_tree(TreeNode* root);
void write_tree_in_order(TreeNode* root, FILE* f); // Para dados2.txt

#endif // ARVORE_BINARIA_H