#include "ArvoreBinaria.h"
#include <stdlib.h>

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
        return create_tree_node(cpf, dataNode);
    }
    
    if (cpf < root->cpf) {
        root->left = insert_abo(root->left, cpf, dataNode);
    } else if (cpf > root->cpf) {
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
    if (root == NULL) return 0; // Altura de arvore vazia é 0? Ou altura em niveis?
    // Exemplo PDF: Raiz apenas -> Altura 1. 
    // Exemplo PDF: 7 nodos balanceados -> Altura 3.
    // Se 7 nodos (cheia) tem altura 3, entao a contagem é por niveis (1-based) ou arestas?
    // log2(7) ~ 2.8 -> 3.
    // Com 1 no: 1.
    return 1 + max_val(tree_height(root->left), tree_height(root->right));
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
        
        // Gravar apenas CPF ou linha completa?
        // PDF: "Gravar o conteúdo da árvore binária ordenada em modo “Em Ordem” (só CPF)"
        // Mas logo abaixo diz: "Descrição detalhada e exemplo do arquivo dados2.txt em anexo"
        // Geralmente "só CPF" quer dizer a chave, mas talvez o formato seja CPF-DV Nome...
        // Pela lógica do exercicio ("indice"), talvez seja só o CPF mesmo.
        // Porem, o objetivo é verificar a ordenação.
        // Vou assumir imprimir o CPF formatado e talvez o nome para conferencia, 
        // MAS O ENUNCIADO DIZ "(só CPF)". Vou imprimir CPF-DV para ficar consistente.
        // Atualização: O PDF mostra no exemplo de saida de tela os dados completos.
        // Vamos olhar o context do usuario se tem dados2.txt exemplo... nao tem.
        // Vou imprimir CPF-DV Nome Data para garantir, pois "só CPF" pode significar "ordenado só por CPF".
        // Se for estrito, seria só o numero.
        // Vou usar o formato padrão de saida de dados: 000000001-01 ...
        
        if (root->dataNode) {
             fprintf(f, "%09lld\n", root->cpf);
        }
        
        write_tree_in_order(root->right, f);
    }
}