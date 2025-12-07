#include "ArvoreBinaria.h"
#include <stdlib.h>
#include <stdio.h>

// Cria um nó da árvore. O nó guarda CPF e o ponteiro para a lista.
TreeNode* create_tree_node(long long cpf, ListNode* dataNode) {
    fprintf(stderr, "DEBUG: [Arvore] Criando no arvore CPF %09lld\n", cpf);
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
        fprintf(stderr, "DEBUG: [Arvore] Inserindo raiz/folha CPF %09lld\n", cpf);
        return create_tree_node(cpf, dataNode);
    }
    
    if (cpf < root->cpf) {
        fprintf(stderr, "DEBUG: [Arvore] %09lld < %09lld -> ESQUERDA\n", cpf, root->cpf);
        root->left = insert_abo(root->left, cpf, dataNode);
    } else if (cpf > root->cpf) {
        fprintf(stderr, "DEBUG: [Arvore] %09lld > %09lld -> DIREITA\n", cpf, root->cpf);
        root->right = insert_abo(root->right, cpf, dataNode);
    } else {
        fprintf(stderr, "DEBUG: [Arvore] %09lld == %09lld -> IGNORADO (DUPLICADO)\n", cpf, root->cpf);
    }
    // Se igual, ignoramos (CPFs unicos) 
    
    return root;
}

int count_nodes(TreeNode* root) {
    if (root == NULL) return 0;
    // fprintf(stderr, "DEBUG: [Arvore] Contando no CPF %09lld\n", root->cpf); // Muito verboso para arvores grandes
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

// Função auxiliar de maximo
int max_val(int a, int b) { return (a > b) ? a : b; }

int tree_height(TreeNode* root) {
    if (root == NULL) return 0; 
    return 1 + max_val(tree_height(root->left), tree_height(root->right));
}

TreeNode* get_min_node(TreeNode* root) {
    fprintf(stderr, "DEBUG: [Arvore] Buscando MINIMO...\n");
    if (root == NULL) return NULL;
    TreeNode* current = root;
    while (current->left != NULL) {
        fprintf(stderr, "DEBUG: [Arvore] Descendo esquerda de %09lld\n", current->cpf);
        current = current->left;
    }
    fprintf(stderr, "DEBUG: [Arvore] Minimo encontrado: %09lld\n", current->cpf);
    return current;
}

TreeNode* get_max_node(TreeNode* root) {
    fprintf(stderr, "DEBUG: [Arvore] Buscando MAXIMO...\n");
    if (root == NULL) return NULL;
    TreeNode* current = root;
    while (current->right != NULL) {
        fprintf(stderr, "DEBUG: [Arvore] Descendo direita de %09lld\n", current->cpf);
        current = current->right;
    }
    fprintf(stderr, "DEBUG: [Arvore] Maximo encontrado: %09lld\n", current->cpf);
    return current;
}

void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        // fprintf(stderr, "DEBUG: [Arvore] Liberando no CPF %09lld\n", root->cpf);
        free(root); // Não libera dataNode, pois pertence à Lista
    }
}

// Gravar o conteúdo da árvore binária ordenada em modo “Em Ordem” (só CPF)
// Exemplo PDF: dados2.txt => ...
void write_tree_in_order(TreeNode* root, FILE* f) {
    if (root != NULL) {
        write_tree_in_order(root->left, f);
        
        if (root->dataNode) {
             fprintf(stderr, "DEBUG: [Arvore] Gravando CPF %09lld (In-Order)\n", root->cpf);
             fprintf(f, "%09lld\n", root->cpf);
        }
        
        write_tree_in_order(root->right, f);
    }
}