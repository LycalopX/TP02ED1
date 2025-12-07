#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include "Types.h"

// Estrutura do nó da Lista Duplamente Encadeada (LDE)
typedef struct ListNode
{
    Citizen data;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

// Protótipos
ListNode *create_list_node(Citizen data);
ListNode *insert_list_head(ListNode *head, Citizen data);

void free_list(ListNode *head);
void print_list_to_file(ListNode *head, FILE *f); // Para dados1.txt

#endif // LISTA_H