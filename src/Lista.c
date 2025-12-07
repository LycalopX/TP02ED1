#include "Lista.h"
#include <stdlib.h>
#include <string.h>

ListNode* create_list_node(Citizen data) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (!newNode) {
        perror("Erro memoria ListNode");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

ListNode* insert_list_head(ListNode* head, Citizen data) {
    ListNode* newNode = create_list_node(data);
    if (head != NULL) {
        newNode->next = head;
        head->prev = newNode;
    }
    return newNode; // Novo head
}

void free_list(ListNode* head) {
    ListNode* current = head;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
    }
}

// Grava o conteúdo da lista encadeada LDE onde os dados são inseridos sempre no início
// Formato CSV simples ou similar ao print? PDF diz: "Descrição detalhada e exemplo do arquivo dados1.txt em anexo"
// Assumindo formato padrão de saída baseado no código anterior e PDF: CPF-DV DD/MM/AAAA Nome
void print_list_to_file(ListNode* head, FILE* f) {
    ListNode* current = head;
    while (current != NULL) {
        // PDF exemplo: 000000001-01 01/01/2001 Fulano1
        fprintf(f, "%s-%s %02d/%02d/%04d %s\n", 
            current->data.cpf, current->data.dv, 
            current->data.dia, current->data.mes, current->data.ano, 
            current->data.nome);
            
        current = current->next;
    }
}