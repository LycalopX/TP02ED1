#ifndef ARVORE_BINARIA_H
#define ARVORE_BINARIA_H

#include <stdio.h>
#include "Lista.h" // Precisa conhecer ListNode para o ponteiro

// Estrutura do nó da Árvore Binária Ordenada (ABO)
// "Os nodos da ABO contem apenas o CPF... porém contem também um link de acesso rápido... na LDE"
typedef struct TreeNode {
    long long cpf;       // Chave de busca
    ListNode* dataNode;  // Link direto para os dados na LDE
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// Protótipos
TreeNode* create_tree_node(long long cpf, ListNode* dataNode);
TreeNode* insert_abo(TreeNode* root, long long cpf, ListNode* dataNode);
TreeNode* search_abo(TreeNode* root, long long cpf);
void free_tree(TreeNode* root);

// Funções auxiliares pedidas
int count_nodes(TreeNode* root);
int tree_height(TreeNode* root);
TreeNode* get_min_node(TreeNode* root); // 1o. na ordem
TreeNode* get_max_node(TreeNode* root); // Ultimo na ordem

// Escrita em arquivo
void write_tree_in_order(TreeNode* root, FILE* f); // Para dados2.txt

#endif // ARVORE_BINARIA_H