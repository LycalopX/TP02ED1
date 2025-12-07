#include "ArvoreBinaria.h"
#include <stdlib.h>
#include <stdio.h>

// Cria um nó da árvore. O nó guarda CPF e o ponteiro para a lista.
TreeNode* create_tree_node(long long cpf, ListNode* dataNode) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        perror("Erro memoria TreeNode");
        exit(EXIT_FAILURE);
    }
    node->cpf = cpf;
    node->dataNode = dataNode;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Inserção em ABO (Não Balanceada)
TreeNode* insert_abo(TreeNode* root, long long cpf, ListNode* dataNode) {
    if (root == NULL) {
        fprintf(stderr, "DEBUG: Inserindo NOVO NO %lld (Nivel ?)\n", cpf);
        return create_tree_node(cpf, dataNode);
    }
    
    if (cpf < root->cpf) {
        fprintf(stderr, "DEBUG: %lld < %lld (Indo Esquerda)\n", cpf, root->cpf);
        root->left = insert_abo(root->left, cpf, dataNode);
    } else if (cpf > root->cpf) {
        fprintf(stderr, "DEBUG: %lld > %lld (Indo Direita)\n", cpf, root->cpf);
        root->right = insert_abo(root->right, cpf, dataNode);
    }
    // Se igual, ignoramos (CPFs unicos) 
    
    return root;
}

int count_nodes(TreeNode* root) {
    if (root == NULL) return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// Função auxiliar de maximo
int max_val(int a, int b) { return (a > b) ? a : b; }

int tree_height(TreeNode* root) {
    if (root == NULL) return 0; 
    int h_left = tree_height(root->left);
    int h_right = tree_height(root->right);
    int my_h = 1 + max_val(h_left, h_right);
    // fprintf(stderr, "DEBUG: Altura no CPF %lld: %d (Esq=%d, Dir=%d)\n", root->cpf, my_h, h_left, h_right);
    return my_h;
}

TreeNode* get_min_node(TreeNode* root) {
    if (root == NULL) return NULL;
    TreeNode* current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

TreeNode* get_max_node(TreeNode* root) {
    if (root == NULL) return NULL;
    TreeNode* current = root;
    while (current->right != NULL) {
        current = current->right;
    }
    return current;
}

void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root); // Não libera dataNode, pois pertence à Lista
    }
}

// Gravar o conteúdo da árvore binária ordenada em modo “Em Ordem” (só CPF)
// Exemplo PDF: dados2.txt => ...
void write_tree_in_order(TreeNode* root, FILE* f) {
    if (root != NULL) {
        write_tree_in_order(root->left, f);
        
        if (root->dataNode) {
             fprintf(f, "%09lld\n", root->cpf);
        }
        
        write_tree_in_order(root->right, f);
    }
}