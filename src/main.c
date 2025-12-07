#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ArvoreBinaria.h"
#include "Lista.h"
#include "Types.h"

// --- Helper Functions ---

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

int read_non_empty_line(FILE* f, char* buffer, int size) {
    while (fgets(buffer, size, f) != NULL) {
        trim_string(buffer);
        if (strlen(buffer) > 0) return 1;
    }
    return 0;
}

// Tries to parse a line into a Citizen struct using multiple formats
int parse_citizen(char* line, Citizen* c) {
    char temp_line[1024];
    strcpy(temp_line, line);
    normalize_separators(temp_line);

    long long cpf_base;
    char nome_temp[100];
    int fields;

    // Try format 1: Formatted date/cpf (e.g. "000000001-01 01/01/2001 Name")
    // Note: %50[^\n] reads the rest of the line including spaces
    fields = sscanf(temp_line, "%lld-%d %d/%d/%d %50[^\n]", 
                    &cpf_base, &c->dv, &c->dia, &c->mes, &c->ano, nome_temp);

    // Try format 2: Space separated (e.g. "000000001 01 01 01 2001 Name")
    if (fields != 6) {
        fields = sscanf(temp_line, "%lld %d %d %d %d %50[^\n]", 
                        &cpf_base, &c->dv, &c->dia, &c->mes, &c->ano, nome_temp);
    }

    if (fields == 6) {
        c->cpf = cpf_base;
        trim_string(nome_temp);
        strncpy(c->nome, nome_temp, 50);
        c->nome[50] = '\0';
        return 1; // Success
    }

    return 0; // Failure
}

// --- Main ---

int main() {
    // Disable output buffering for consistent testing
    setvbuf(stdout, NULL, _IONBF, 0);

    int option;
    char buffer[1024]; 

    // Read option
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    if (sscanf(buffer, "%d", &option) != 1) return 1;

    // Open database
    FILE* f = fopen("basedados.txt", "r");
    if (!f) {
        f = fopen("basedados.csv", "r");
        if (!f) {
            printf("Erro ao abrir basedados.txt\n"); 
            return 1;
        }
    }

    ListNode* listHead = NULL;
    TreeNode* treeRoot = NULL;

    // Process file
    while (read_non_empty_line(f, buffer, sizeof(buffer))) {
        // Check for terminator
        if (strncmp(buffer, "-1", 2) == 0) break;

        Citizen c;
        
        // Attempt single-line parse
        if (parse_citizen(buffer, &c)) {
            listHead = insert_list_head(listHead, c);
            treeRoot = insert_abo(treeRoot, c.cpf, listHead);
            continue;
        }

        // Fallback: Multi-line parse logic
        // If single line parse failed, assume 'buffer' contains just the CPF
        long long cpf_base;
        if (sscanf(buffer, "%lld", &cpf_base) != 1) continue;
        
        c.cpf = cpf_base;

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

        listHead = insert_list_head(listHead, c);
        treeRoot = insert_abo(treeRoot, c.cpf, listHead);
    }
    fclose(f);

    // Generate output files
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

    // Handle Option 2 (Cat files)
    if (option == 2) {
        system("cat dados1.txt");
        system("cat dados2.txt");
    }

    // Print Statistics
    int count = count_nodes(treeRoot);
    int h = tree_height(treeRoot);

    printf("%d\n", count);
    printf("%d\n", h);
    
    // Print Min/Max
    TreeNode* minNode = get_min_node(treeRoot);
    print_node_data(minNode);
    
    TreeNode* maxNode = get_max_node(treeRoot);
    print_node_data(maxNode);

    // Cleanup
    free_tree(treeRoot); 
    free_list(listHead); 

    return 0;
}
