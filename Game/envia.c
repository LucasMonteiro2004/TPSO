#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAMANHO_MAX 3000
#define TAM_NOME 50

char *nome;

void enviaLabirinto() {
    int fd;
    char *pipeNomeado = "/tmp/meu_pipe"; // Nome do pipe para comunicação

    mkfifo(pipeNomeado, 0666); // Criação do pipe

    char mensagem_enviada[TAMANHO_MAX];

    FILE *file = fopen("labirinto1.txt", "r");
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

    int bytesRead = read(fd, nome, TAM_NOME - 1);
    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return;
    }

    printf("%s\n", nome);

    close(fd);
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
