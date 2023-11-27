#include "header.h"

void enviaLabirinto() {
    int fd;
    mkfifo(pipeMotor, 0666); // Criação do pipe
    char lab[MAX_LINHAS][MAX_COLUNAS + 1]; // +1 para o caractere nulo

    FILE *file = fopen(arquivo, "rt");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Lê o número de linhas e colunas do arquivo
    fscanf(file, "%d %d", &linhas, &colunas);

    // Verifica se os valores lidos são válidos
    if (linhas > MAX_LINHAS || colunas > MAX_COLUNAS) {
        fprintf(stderr, "Tamanho da matriz excede os limites.\n");
        fclose(file);
        return;
    }

    // Lê os elementos do arquivo e os armazena na matriz
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(file, "%d", &lab[i][j]);
        }
    }
    
    close(fd);
    fclose(file);
}
