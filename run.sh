#!/bin/bash

# Detecta o sistema operacional
OS=$(uname -s)

# Define o comando de execução com ou sem Valgrind
if [ "$OS" = "Linux" ]; then
    echo "Detectado Linux. Executando com Valgrind."
    RUN_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./programaTrab"
else
    echo "Detectado $OS. Executando sem Valgrind."
    RUN_CMD="./programaTrab"
fi

#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose 

# Limpa os arquivos objeto e o executável antes de compilar
make clean

# Compila o projeto
make

echo "--- Executando Testes ---"

INPUT_FILE="Casos de Teste - Trabalho TP02 - CPF com Arvore ABO e Lista LDE/1.in"
EXPECTED_OUTPUT_FILE="Casos de Teste - Trabalho TP02 - CPF com Arvore ABO e Lista LDE/1.out"
OUTPUT_FILE=$(mktemp)

echo ">> Teste 1"
$RUN_CMD < "$INPUT_FILE" > "$OUTPUT_FILE"

if diff -q "$OUTPUT_FILE" "$EXPECTED_OUTPUT_FILE"; then
    echo "Teste 1: PASSOU"
else
    echo "--- Saída Esperada ---"
    cat "$EXPECTED_OUTPUT_FILE"
    echo "--- Saída Obtida ---"
    cat "$OUTPUT_FILE"
    echo "----------------------"
fi

rm "$OUTPUT_FILE"

echo "--- Fim dos Testes ---"

# silenciosamente
make clean &> /dev/null
make zip &> /dev/null