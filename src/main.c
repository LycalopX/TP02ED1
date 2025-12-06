#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvoreBinaria.h"
#include "Lista.h"
#include "Types.h"

// Função auxiliar para imprimir um nó no formato especificado
void print_node_data(TreeNode* node) {
    if (node && node->dataNode) {
        Citizen c = node->dataNode->data;
        printf("%09lld-%02d %02d/%02d/%04d %s\n", 
               c.cpf, c.dv, c.dia, c.mes, c.ano, c.nome);
    }
}

int main() {
    int option;
    if (scanf("%d", &option) != 1) {
        return 1;
    }

    FILE* f = fopen("basedados.txt", "r");
    if (!f) {
        printf("Erro ao abrir basedados.txt\n"); 
        return 1;
    }

    ListNode* listHead = NULL;
    TreeNode* treeRoot = NULL;

    long long cpf_base;
    int cpf_dv;
    int dia, mes, ano;
    char nome[51];
    
    // Leitura robusta usando fscanf
    while (1) {
        long long temp_cpf;
        // Tenta ler o primeiro numero (CPF base)
        if (fscanf(f, "%lld", &temp_cpf) != 1) {
            break; // Fim do arquivo ou erro
        }
        
        if (temp_cpf == -1) {
            break; // Condição de parada especificada
        }
        
        cpf_base = temp_cpf;
        
        // Lê o restante da linha: DV Dia Mes Ano Nome
        // O espaço antes de %d consome whitespace se necessário, mas %d já faz isso.
        // O nome é lido com %[^
] para pegar até o fim da linha (se tiver sobrenome).
        // Se o arquivo tiver espaços extras, o scanf cuida.
        if (fscanf(f, "%d %d %d %d %50[^
]", &cpf_dv, &dia, &mes, &ano, nome) != 5) {
            // Se falhar em ler o resto da linha, ignora ou para?
            break; 
        }
        
        Citizen c;
        c.cpf = cpf_base;
        c.dv = cpf_dv;
        c.dia = dia; 
        c.mes = mes; 
        c.ano = ano;
        strcpy(c.nome, nome);
        
        // 1. Inserir na LDE (Inicio)
        listHead = insert_list_head(listHead, c);
        
        // 2. Inserir na ABO
        treeRoot = insert_abo(treeRoot, c.cpf, listHead);
    }
    fclose(f);

    // Gerar arquivos de saída
    FILE* out1 = fopen("dados1.txt", "w");
    if (out1) {
        print_list_to_file(listHead, out1);
        fclose(out1);
    }
    
    FILE* out2 = fopen("dados2.txt", "w");
    if (out2) {
        write_tree_in_order(treeRoot, out2);
        fclose(out2);
    }

    // Funcionalidade 2: Executar comandos extras (antes dos stats, conforme solicitado anteriormente?)
    // O usuário reclamou da saída do teste 2. Vamos ver a saída esperada do teste 3 que tem func 2.
    // Saída esperada T3:
    // <cat dados1>
    // <cat dados2>
    // Stats...
    // Então sim, Func 2 vem ANTES dos stats.
    
    if (option == 2) {
        system("cat dados1.txt");
        system("cat dados2.txt");
    }

    // Saída na tela (Stats)
    printf("%d\n", count_nodes(treeRoot));
    printf("%d\n", tree_height(treeRoot));
    
    // Dados do 1o nodo considerando ordenação (Menor CPF)
    TreeNode* minNode = get_min_node(treeRoot);
    print_node_data(minNode);
    
    // Dados do ultimo nodo considerando ordenação (Maior CPF)
    TreeNode* maxNode = get_max_node(treeRoot);
    print_node_data(maxNode);

    // Limpeza
    free_tree(treeRoot); 
    free_list(listHead); 

    return 0;
}
