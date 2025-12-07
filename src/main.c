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
        fprintf(stderr, "DEBUG: [Main] Exibindo dados do no CPF %09lld\n", c.cpf);
        printf("%09lld-%02d %02d/%02d/%04d %s\n", 
               c.cpf, c.dv, c.dia, c.mes, c.ano, c.nome);
    } else {
        fprintf(stderr, "DEBUG: [Main] Tentativa de exibir no NULL ou vazio.\n");
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
    fprintf(stderr, "DEBUG: --- INICIO DO PROGRAMA ---\n");
    setvbuf(stdout, NULL, _IONBF, 0);

    int option;
    char buffer[1024]; 

    fprintf(stderr, "DEBUG: Aguardando input da opcao...\n");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "DEBUG: Erro ao ler opcao da entrada padrao.\n");
        return 1;
    }
    if (sscanf(buffer, "%d", &option) != 1) {
        fprintf(stderr, "DEBUG: Erro ao fazer parse da opcao.\n");
        return 1;
    }
    fprintf(stderr, "DEBUG: Opcao selecionada: %d\n", option);

    FILE* f = fopen("basedados.txt", "r");
    if (!f) {
        fprintf(stderr, "DEBUG: Falha ao abrir basedados.txt. Tentando basedados.csv...\n");
        f = fopen("basedados.csv", "r");
        if (!f) {
            fprintf(stderr, "DEBUG: Falha ao abrir basedados.csv tambem. Abortando.\n");
            printf("Erro ao abrir basedados.txt\n"); 
            return 1;
        }
        fprintf(stderr, "DEBUG: basedados.csv aberto com sucesso.\n");
    } else {
        fprintf(stderr, "DEBUG: basedados.txt aberto com sucesso.\n");
    }

    /*
    // --- DUMP DO ARQUIVO PARA DEBUG ---
    // COMENTADO: Isso reseta o ponteiro do arquivo se nao fizer rewind corretamente ou se for pipe. 
    // Melhor confiar na leitura linha a linha.
    fprintf(stderr, "DEBUG: --- CONTEUDO DO ARQUIVO (START) ---\n");
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        fputc(ch, stderr);
    }
    fprintf(stderr, "\nDEBUG: --- CONTEUDO DO ARQUIVO (END) ---\n");
    rewind(f);
    // ----------------------------------
    */

    ListNode* listHead = NULL;
    TreeNode* treeRoot = NULL;
    int record_count = 0;

    fprintf(stderr, "DEBUG: Iniciando loop de leitura...\n");

    while (read_non_empty_line(f, buffer, sizeof(buffer))) {
        fprintf(stderr, "DEBUG: ----------------------------------------------------------------\n");
        fprintf(stderr, "DEBUG: [Lendo Registro %d] Linha bruta lida: '%s'\n", record_count+1, buffer);

        // Verifica terminador
        if (strncmp(buffer, "-1", 2) == 0) {
            fprintf(stderr, "DEBUG: Terminador '-1' encontrado. Parando leitura.\n");
            break;
        }

        char temp_line[1024];
        strcpy(temp_line, buffer);
        normalize_separators(temp_line);

        Citizen c;
        long long cpf_base;
        char nome_temp[100];
        
        // Tenta ler como LINHA ÚNICA (Com separadores visualmente formatados: - e /)
        int fields = sscanf(temp_line, "%lld-%d %d/%d/%d %50[^\n]", 
               &cpf_base, &c.dv, &c.dia, &c.mes, &c.ano, nome_temp);
        
        if (fields != 6) {
             // Fallback: Tenta ler com espaços (caso normalize tenha limpado ou seja formato simples)
             fields = sscanf(temp_line, "%lld %d %d %d %d %50[^\n]", 
               &cpf_base, &c.dv, &c.dia, &c.mes, &c.ano, nome_temp);
        }

        fprintf(stderr, "DEBUG: Teste Linha Unica: sscanf retornou %d campos.\n", fields);

        if (fields == 6) {
            // SUCESSO LINHA ÚNICA
            fprintf(stderr, "DEBUG: Detectado formato LINHA UNICA.\n");
            c.cpf = cpf_base;
            trim_string(nome_temp);
            strncpy(c.nome, nome_temp, 50);
            c.nome[50] = '\0';
        } else {
            // FALHA: MULTI-LINHAS
            fprintf(stderr, "DEBUG: Detectado formato MULTI-LINHAS (assumido).\n");
            
            // O buffer atual já é a linha do CPF
            if (sscanf(buffer, "%lld", &cpf_base) != 1) {
                fprintf(stderr, "DEBUG: FALHA GRAVE: Nao foi possivel ler CPF da linha '%s'. Pulando.\n", buffer);
                continue;
            }
            c.cpf = cpf_base;
            fprintf(stderr, "DEBUG: CPF lido: %lld\n", c.cpf);

            // Lê próximos campos IGNORANDO LINHAS EM BRANCO
            if (!read_non_empty_line(f, buffer, sizeof(buffer))) { fprintf(stderr, "DEBUG: EOF inesperado lendo DV.\n"); break; }
            c.dv = atoi(buffer);
            fprintf(stderr, "DEBUG: DV lido: %d\n", c.dv);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) { fprintf(stderr, "DEBUG: EOF inesperado lendo Dia.\n"); break; }
            c.dia = atoi(buffer);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) { fprintf(stderr, "DEBUG: EOF inesperado lendo Mes.\n"); break; }
            c.mes = atoi(buffer);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) { fprintf(stderr, "DEBUG: EOF inesperado lendo Ano.\n"); break; }
            c.ano = atoi(buffer);

            if (!read_non_empty_line(f, buffer, sizeof(buffer))) { fprintf(stderr, "DEBUG: EOF inesperado lendo Nome.\n"); break; }
            trim_string(buffer);
            strncpy(c.nome, buffer, 50);
            c.nome[50] = '\0';
            fprintf(stderr, "DEBUG: Nome lido: %s\n", c.nome);
        }

        fprintf(stderr, "DEBUG: Dados Completos: CPF=%lld-%d DATA=%02d/%02d/%04d NOME='%s'\n", 
                c.cpf, c.dv, c.dia, c.mes, c.ano, c.nome);

        fprintf(stderr, "DEBUG: Chamando insert_list_head...\n");
        listHead = insert_list_head(listHead, c);
        
        fprintf(stderr, "DEBUG: Chamando insert_abo...\n");
        treeRoot = insert_abo(treeRoot, c.cpf, listHead);
        
        record_count++;
    }
    fclose(f);
    fprintf(stderr, "DEBUG: Leitura concluida. Total registros processados: %d\n", record_count);

    fprintf(stderr, "DEBUG: Gerando arquivos de saida...\n");
    FILE* out1 = fopen("dados1.txt", "w");
    if (out1) {
        print_list_to_file(listHead, out1);
        fclose(out1);
        fprintf(stderr, "DEBUG: dados1.txt gerado.\n");
    } else {
        fprintf(stderr, "DEBUG: Erro ao criar dados1.txt\n");
    }
    
    FILE* out2 = fopen("dados2.txt", "w");
    if (out2) {
        write_tree_in_order(treeRoot, out2);
        fclose(out2);
        fprintf(stderr, "DEBUG: dados2.txt gerado.\n");
    } else {
         fprintf(stderr, "DEBUG: Erro ao criar dados2.txt\n");
    }

    if (option == 2) {
        fprintf(stderr, "DEBUG: Executando system(cat...)\n");
        system("cat dados1.txt");
        system("cat dados2.txt");
    }

    fprintf(stderr, "DEBUG: Calculando estatisticas da arvore...\n");
    int count = count_nodes(treeRoot);
    int h = tree_height(treeRoot);
    fprintf(stderr, "DEBUG: Stats Arvore: N=%d, H=%d\n", count, h);

    printf("%d\n", count);
    printf("%d\n", h);
    
    fprintf(stderr, "DEBUG: Buscando Minimo e Maximo...\n");
    TreeNode* minNode = get_min_node(treeRoot);
    if(minNode) fprintf(stderr, "DEBUG: Minimo no found: %09lld\n", minNode->cpf);
    print_node_data(minNode);
    
    TreeNode* maxNode = get_max_node(treeRoot);
    if(maxNode) fprintf(stderr, "DEBUG: Maximo no found: %09lld\n", maxNode->cpf);
    print_node_data(maxNode);

    fprintf(stderr, "DEBUG: Liberando memoria...\n");
    free_tree(treeRoot); 
    free_list(listHead); 
    
    fprintf(stderr, "DEBUG: --- FIM DO PROGRAMA ---\n");

    return 0;
}