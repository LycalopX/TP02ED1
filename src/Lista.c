#include "Lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ListNode* create_list_node(Citizen data) {
    fprintf(stderr, "DEBUG: [Lista] Criando no para CPF %09lld\n", data.cpf);
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (!newNode) {
        perror("Erro memoria ListNode");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    fprintf(stderr, "DEBUG: [Lista] No criado em %p\n", (void*)newNode);
    return newNode;
}

ListNode* insert_list_head(ListNode* head, Citizen data) {
    fprintf(stderr, "DEBUG: [Lista] Inserindo no inicio. Head atual: %p\n", (void*)head);
    ListNode* newNode = create_list_node(data);
    if (head != NULL) {
        newNode->next = head;
        head->prev = newNode;
    }
    fprintf(stderr, "DEBUG: [Lista] Novo Head: %p\n", (void*)newNode);
    return newNode; // Novo head
}

void free_list(ListNode* head) {
    fprintf(stderr, "DEBUG: [Lista] Liberando lista a partir de %p\n", (void*)head);
    ListNode* current = head;
    int count = 0;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        // fprintf(stderr, "DEBUG: [Lista] Liberando no %p (CPF %09lld)\n", (void*)temp, temp->data.cpf);
        free(temp);
        count++;
    }
    fprintf(stderr, "DEBUG: [Lista] Lista liberada. Total de nos removidos: %d\n", count);
}

// Grava o conteúdo da lista encadeada LDE onde os dados são inseridos sempre no início
// Formato CSV simples ou similar ao print? PDF diz: "Descrição detalhada e exemplo do arquivo dados1.txt em anexo"
// Assumindo formato padrão de saída baseado no código anterior e PDF: CPF-DV DD/MM/AAAA Nome
void print_list_to_file(ListNode* head, FILE* f) {
    fprintf(stderr, "DEBUG: [Lista] Escrevendo lista no arquivo...\n");
    ListNode* current = head;
    int count = 0;
    while (current != NULL) {
        // PDF exemplo: 000000001-01 01/01/2001 Fulano1
        // Note que CPF tem zeros a esquerda
        // data.cpf é long long
        long long base = current->data.cpf;
        int dv = current->data.dv;
        
        fprintf(f, "%09lld-%02d %02d/%02d/%04d %s\n", 
            base, dv, 
            current->data.dia, current->data.mes, current->data.ano, 
            current->data.nome);
            
        current = current->next;
        count++;
    }
    fprintf(stderr, "DEBUG: [Lista] Total de registros gravados: %d\n", count);
}