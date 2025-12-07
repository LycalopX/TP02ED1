#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ArvoreBinaria.h"
#include "Lista.h"
#include "Types.h"

// --- Helper Functions ---

// Clean token like the reference implementation: truncate at first non-printable/space
void clean_token(char* str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        if (str[i] <= 32) { // 32 is space
            str[i] = '\0';
            break; 
        }
    }
}

void print_node_data(TreeNode* node) {
    if (node && node->dataNode) {
        Citizen c = node->dataNode->data;
        // Format matches reference: %s-%s ...
        printf("%s-%s %02d/%02d/%04d %s\n", 
               c.cpf, c.dv, c.dia, c.mes, c.ano, c.nome);
    }
}

// Remove newline only (safer trim)
void trim_newline(char* str) {
    size_t len = strlen(str);
    while(len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        len--;
    }
}

int read_non_empty_line(FILE* f, char* buffer, int size) {
    while (fgets(buffer, size, f) != NULL) {
        trim_newline(buffer);
        if (strlen(buffer) > 0) return 1;
    }
    return 0;
}

// --- Main ---

int main() {
    // Disable output buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    int option;
    char buffer[1024]; 

    // Read option
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 1;
    if (sscanf(buffer, "%d", &option) != 1) return 1;

    // Open database - Try CSV first as per reference
    FILE* f = fopen("basedados.csv", "r");
    if (!f) {
        f = fopen("basedados.txt", "r");
        if (!f) {
            printf("Erro ao abrir arquivo de dados.\n"); 
            return 1;
        }
    }

    ListNode* listHead = NULL;
    TreeNode* treeRoot = NULL;

    // Process file
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        // Check for terminator
        if (strncmp(buffer, "-1", 2) == 0) break;
        
        // Copy buffer for CSV tokenization
        char lineCopy[1024];
        strcpy(lineCopy, buffer);

        // Try CSV parsing (Reference Strategy)
        char* token = strtok(lineCopy, ",");
        if (token) {
            Citizen c;
            
            // 1. CPF
            clean_token(token);
            strcpy(c.cpf, token);

            // 2. DV
            token = strtok(NULL, ",");
            if (!token) continue; // Invalid line
            clean_token(token);
            strcpy(c.dv, token);

            // 3. Dia
            token = strtok(NULL, ",");
            if (!token) continue;
            c.dia = atoi(token);

            // 4. Mes
            token = strtok(NULL, ",");
            if (!token) continue;
            c.mes = atoi(token);

            // 5. Ano
            token = strtok(NULL, ",");
            if (!token) continue;
            c.ano = atoi(token);

            // 6. Nome
            token = strtok(NULL, ",");
            if (!token) continue;
            clean_token(token); // Reference also cleans name!
            strncpy(c.nome, token, 50);
            c.nome[50] = '\0';

            // Insert
            listHead = insert_list_head(listHead, c);
            treeRoot = insert_abo(treeRoot, c.cpf, listHead);
            continue;
        }

        // Fallback: Multi-line parse logic (Only if not CSV)
        // Original logic adapted for string CPF
        // Assuming current buffer has CPF if not CSV
        
        Citizen c;
        trim_newline(buffer);
        clean_token(buffer); // Clean potential spaces
        strcpy(c.cpf, buffer);

        if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
        clean_token(buffer);
        strcpy(c.dv, buffer);

        if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
        c.dia = atoi(buffer);

        if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
        c.mes = atoi(buffer);

        if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
        c.ano = atoi(buffer);

        if (!read_non_empty_line(f, buffer, sizeof(buffer))) break;
        clean_token(buffer);
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
