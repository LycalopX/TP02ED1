#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ArvoreBinaria.h"
#include "Lista.h"
#include "Types.h"

void print_node_data(TreeNode* node) {
    if (node && node->dataNode) {
        Citizen c = node->dataNode->data;
        printf("%09lld-%02d %02d/%02d/%04d %s\n", 
               c.cpf, c.dv, c.dia, c.mes, c.ano, c.nome);
    }
}

void trim_string(char* str) {
    size_t len = strlen(str);
    while(len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        len--;
    }
}

void normalize_separators(char* str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == ',' || str[i] == ';') {
            str[i] = ' ';
        }
    }
}

// Wrapper seguro para ler linha não vazia
int read_non_empty_line(FILE* f, char* buffer, int size) {
    while (fgets(buffer, size, f) != NULL) {
        trim_string(buffer);
        if (strlen(buffer) > 0) return 1; // Leu linha útil
    }
    return 0; // Fim de arquivo
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);

    int option;
    char buffer[1024]; 

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    if (sscanf(buffer, "%d", &option) != 1) {
        return 1;
    }

    FILE* f = fopen("basedados.txt", "r");
    if (!f) {
        f = fopen("basedados.csv", "r");
        if (!f) {
            printf("Erro ao abrir basedados.txt\n"); 
            return 1;
        }
    }

    // --- DEBUG: DUMP FILE CONTENT ---
    fprintf(stderr, "DEBUG: --- FILE START ---\n");
    int line_num = 1;
    char dump_buffer[1024];
    while (fgets(dump_buffer, sizeof(dump_buffer), f)) {
        // Remove newline for display
        size_t len = strlen(dump_buffer);
        if (len > 0 && dump_buffer[len-1] == '\n') dump_buffer[len-1] = '\0';
        fprintf(stderr, "L%03d: [%s]\n", line_num++, dump_buffer);
    }
    fprintf(stderr, "DEBUG: --- FILE END ---\n");
    rewind(f);
    // --------------------------------

    ListNode* listHead = NULL;
    TreeNode* treeRoot = NULL;

    while (read_non_empty_line(f, buffer, sizeof(buffer))) {
        // Verifica terminador
        if (strncmp(buffer, "-1", 2) == 0) {
            break;
        }

        char temp_line[1024];
        strcpy(temp_line, buffer);
        normalize_separators(temp_line);

        Citizen c;
        long long cpf_base;
        char nome_temp[100];
        
        // Tenta ler como LINHA ÚNICA
        // Tenta primeiro com formato formatado (hífens e barras)
        // Nota: "%50[^\n]" lê até o fim da linha
        int fields = sscanf(temp_line, "%lld-%d %d/%d/%d %50[^\n]", &cpf_base, &c.dv, &c.dia, &c.mes, &c.ano, nome_temp);

        if (fields != 6) {
             // Fallback: Tenta ler com espaços simples
             fields = sscanf(temp_line, "%lld %d %d %d %d %50[^\n]", &cpf_base, &c.dv, &c.dia, &c.mes, &c.ano, nome_temp);
        }

        if (fields == 6) {
            c.cpf = cpf_base;
            trim_string(nome_temp);
            strncpy(c.nome, nome_temp, 50);
            c.nome[50] = '\0';
        } else {
            // FALHA: MULTI-LINHAS
            // O buffer atual já é a linha do CPF
            if (sscanf(buffer, "%lld", &cpf_base) != 1) {
                continue;
            }
            c.cpf = cpf_base;

            // Lê próximos campos IGNORANDO LINHAS EM BRANCO
            if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
            c.dv = atoi(buffer);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
            c.dia = atoi(buffer);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
            c.mes = atoi(buffer);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
            c.ano = atoi(buffer);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
            trim_string(buffer);
            strncpy(c.nome, buffer, 50);
            c.nome[50] = '\0';
        }

        listHead = insert_list_head(listHead, c);
        treeRoot = insert_abo(treeRoot, c.cpf, listHead);
    }
    fclose(f);

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

    if (option == 2) {
        system("cat dados1.txt");
        system("cat dados2.txt");
    }

    int count = count_nodes(treeRoot);
    int h = tree_height(treeRoot);

    printf("%d\n", count);
    printf("%d\n", h);
    
    TreeNode* minNode = get_min_node(treeRoot);
    print_node_data(minNode);
    
    TreeNode* maxNode = get_max_node(treeRoot);
    print_node_data(maxNode);

    free_tree(treeRoot); 
    free_list(listHead); 
    return 0;
}