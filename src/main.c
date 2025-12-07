#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

// Helper para remover newline e carriage return
void trim_string(char* str) {
    size_t len = strlen(str);
    while(len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        len--;
    }
}

// Substitui vírgulas por espaços para facilitar o sscanf
void normalize_separators(char* str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == ',') {
            str[i] = ' ';
        }
    }
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    int option;
    char buffer[1024]; 

    // Ler opcao
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    if (sscanf(buffer, "%d", &option) != 1) return 1;

    FILE* f = fopen("basedados.txt", "r");
    if (!f) {
        // Fallback apenas para teste local se necessario
        f = fopen("basedados.csv", "r");
        if (!f) {
            printf("Erro ao abrir basedados.txt\n"); 
            return 1;
        }
    }

    ListNode* listHead = NULL;
    TreeNode* treeRoot = NULL;

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        trim_string(buffer);
        if (strlen(buffer) == 0) continue;

        // Verifica terminador
        if (strncmp(buffer, "-1", 2) == 0) break;

        // Cria uma cópia do buffer para tentar parsear como Linha Única
        char temp_line[1024];
        strcpy(temp_line, buffer);
        normalize_separators(temp_line); // Transforma virgulas em espaços

        Citizen c;
        long long cpf_base;
        char nome_temp[100];
        
        // Tenta ler os 6 campos de uma vez (Formato CSV ou Linha Única)
        // O espaço antes de %50 ignora whitespaces antes do nome
        int fields = sscanf(temp_line, "%lld %d %d %d %d %50[^
]", 
               &cpf_base, &c.dv, &c.dia, &c.mes, &c.ano, nome_temp);

        if (fields == 6) {
            // --- SUCESSO: É LINHA ÚNICA (CSV ou Espaços) ---
            c.cpf = cpf_base;
            trim_string(nome_temp);
            strncpy(c.nome, nome_temp, 50);
            c.nome[50] = '\0';
        } else {
            // --- FALHA: TENTAR FORMATO MULTI-LINHAS ---
            // Se falhou, assume que a linha lida era apenas o CPF
            if (sscanf(buffer, "%lld", &cpf_base) != 1) {
                // Se nem CPF é, pula
                continue;
            }
            c.cpf = cpf_base;

            // Lê as próximas 5 linhas obrigatoriamente
            // 2. DV
            if (fgets(buffer, sizeof(buffer), f) == NULL) break;
            c.dv = atoi(buffer);

            // 3. Dia
            if (fgets(buffer, sizeof(buffer), f) == NULL) break;
            c.dia = atoi(buffer);

            // 4. Mes
            if (fgets(buffer, sizeof(buffer), f) == NULL) break;
            c.mes = atoi(buffer);

            // 5. Ano
            if (fgets(buffer, sizeof(buffer), f) == NULL) break;
            c.ano = atoi(buffer);

            // 6. Nome
            if (fgets(buffer, sizeof(buffer), f) == NULL) break;
            trim_string(buffer);
            strncpy(c.nome, buffer, 50);
            c.nome[50] = '\0';
        }

        // Inserir nas estruturas
        listHead = insert_list_head(listHead, c);
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

    // Funcionalidade 2: Executar comandos extras (Se solicitado e APÓS a geração dos arquivos)
    // O PDF diz: "2 <enter> => Executar ao final do programa 2 linhas de código adicionais..."
    if (option == 2) {
        system("cat dados1.txt");
        system("cat dados2.txt");
    }

    // Saída na tela (Stats)
    printf("%d\n", count_nodes(treeRoot));
    printf("%d\n", tree_height(treeRoot));
    
    // Dados do 1o nodo (Menor CPF)
    TreeNode* minNode = get_min_node(treeRoot);
    print_node_data(minNode);
    
    // Dados do ultimo nodo (Maior CPF)
    TreeNode* maxNode = get_max_node(treeRoot);
    print_node_data(maxNode);

    // Limpeza
    free_tree(treeRoot); 
    free_list(listHead); 

    return 0;
}