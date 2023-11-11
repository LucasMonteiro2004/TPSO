#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ncurses.h>

#define TAMANHO_MAX 3000
#define TAM_NOME 10

char *username;

void recebeLabirinto() {
    int fd;
    char *pipeNomeado = "/tmp/meu_pipe";

    fd = open(pipeNomeado, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        return;
    }

    char mensagem_recebida[TAMANHO_MAX];

    int bytesRead = read(fd, mensagem_recebida, TAMANHO_MAX - 1);
    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return;
    }

    mensagem_recebida[bytesRead] = '\0'; // Adiciona terminação da string

    for (int i = 0; i < bytesRead; i++) {
        if (mensagem_recebida[i] == 'F') {
            mensagem_recebida[i] = ' ';
        }
    }

    // Processamento da mensagem recebida
    printf("%s\n", mensagem_recebida);

    close(fd);
}

void enviaNome(char nome[TAM_NOME]){
    int fd;
    char *pipe = "pipeNome";

    mkfifo(pipe, 0666);

    fd = open(pipe, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para escrita do nome!\n");
        return;
    }

    write(fd, nome, strlen(nome));
    close(fd);
}

void enviaJogadas(){
    int fd;
    char *pipe = "PipeJogadas";

    mkfifo(pipe, 0666);

    int jogada = 0;

    fd = open(pipe, O_WRONLY);
    if(fd == -1){
        printf("Erro ao abrir pipe para escrita de jogadas!\n");
        return;
    }

    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
        exit(0);
    }

    if (ch == KEY_RIGHT) {
        jogada = 1;
    } else if (ch == KEY_LEFT) {
        jogada = 2;
    } else if (ch == KEY_UP) {
        jogada = 3;
    } else if (ch == KEY_DOWN) {
        jogada = 4;
    }

    write(fd, &jogada, sizeof(int));
    close(fd);
}

void enviaComandos(){
    int fd;
    char *pipe = "PipeComandos";

    mkfifo(pipe, 0666);

    int jogada = 0;

    fd = open(pipe, O_WRONLY);
    if(fd == -1){
        printf("Erro ao abrir pipe para escrita de comandos!\n");
        return;
    }

    //logica de comandos

    close(fd);
}

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Por favor insira seu nome");
    }

    recebeLabirinto();

    enviaNome(argv[1]);

    return 0;
}
