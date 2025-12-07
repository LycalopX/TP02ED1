#ifndef TYPES_H
#define TYPES_H

typedef struct Citizen {
    char cpf[15]; // Armazenar como string para preservar zeros à esquerda
    char dv[5];   // Armazenar como string
    int dia, mes, ano;
    char nome[51];
} Citizen;

#endif
