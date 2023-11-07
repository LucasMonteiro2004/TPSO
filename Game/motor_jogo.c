#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAMANHO_MAX 3000
#define TAM_NOME 10

char nome[TAM_NOME], linha[80];
int GRID_WIDTH = 0, GRID_HEIGHT = 0;
char *arquivo = "labirinto1.txt";

void calculaDimensoesLabirinto() {
    FILE *file = fopen(arquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    char linha[1000]; // Tamanho máximo de uma linha (ajuste conforme necessário)
    int linhas = 0;

    while (fgets(linha, sizeof(linha), file)) {
        int comprimentoLinha = strlen(linha);
        if (comprimentoLinha > GRID_WIDTH) {
            GRID_WIDTH = comprimentoLinha; // Atualiza a largura se a linha atual for mais longa
        }
        linhas++; // Conta o número de linhas

        int colunasNaLinha = 0;
        char *token = strtok(linha, " "); 

        while (token != NULL) {
            colunasNaLinha++;
            token = strtok(NULL, " ");
        }

        if (colunasNaLinha > GRID_HEIGHT) {
            GRID_HEIGHT= colunasNaLinha; // Atualiza o número máximo de colunas
        }
    }

    GRID_HEIGHT = linhas; // Define a altura como o número de linhas lidas

    fclose(file);
}

void enviaLabirinto() {
    int fd;
    char *pipeNomeado = "/tmp/meu_pipe"; // Nome do pipe para comunicação

    mkfifo(pipeNomeado, 0666); // Criação do pipe

    char mensagem_enviada[TAMANHO_MAX];

    FILE *file = fopen(arquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Leitura do conteúdo do arquivo
    size_t bytesRead = fread(mensagem_enviada, sizeof(char), TAMANHO_MAX - 1, file);
    mensagem_enviada[bytesRead] = '\0'; // Adiciona terminação da string

    fd = open(pipeNomeado, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para escrita!\n");
        fclose(file);
        return;
    }

    // Escreve no pipe
    write(fd, mensagem_enviada, strlen(mensagem_enviada));

    close(fd);
    fclose(file);
}

void NomeUtilizador(){
    int fd;
    char *pipe = "pipeNome";

    fd = open(pipe, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        return;
    }

    int bytesRead = read(fd, nome, TAM_NOME);
    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return;
    }

    printf("%s\n", nome);

    close(fd);
}

int is_obstacle(int x, int y, const char *grid) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return 1; // Fora dos limites da grade é considerado um obstáculo
    }
    return grid[y * GRID_WIDTH + x] == 'X';
}

int is_Fim(int x, int y, const char *grid) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
        return 0; // Fora dos limites da grade não é o fim
    }
    return grid[y * GRID_WIDTH + x] == 'F';
}

void inicializa(){
    printf("\t\t\tMOTOR DE JOGO\n");
    printf("============================================================\n");
    printf("============================================================\n");
}

int main(int argc, char *argv[]) {
    inicializa();
    enviaLabirinto();
    NomeUtilizador();
    return 0;
}
