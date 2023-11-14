#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ncurses.h>

#include "motor_jogo.h"

#define TAMANHO_MAX 3000

player p;

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

void enviaCredenciais(char nome[TAM_NOME]){
    int fd;
    char *pipe = "pipeNome";

    mkfifo(pipe, 0666);

    fd = open(pipe, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para escrita do nome!\n");
        return;
    }

    p.pid = getpid();
    strncpy(p.name, nome, TAM_NOME);

    write(fd, &p, sizeof(p));
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

// Função para validar o comando "users"
int validateUsersCommand(const char *command) {
    if (strcmp(command, "users") == 0) {
        printf("Comando users valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "kick"
int validateKickCommand(const char *command) {
    const char *prefix = "kick ";
    if (strncmp(command, prefix, strlen(prefix)) == 0) {
        printf("Comando kick valido para o jogador: %s\n", command + strlen(prefix));
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "bots"
int validateBotsCommand(const char *command) {
    if (strcmp(command, "bots") == 0) {
        printf("Comando bots valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "bmov"
int validateBmovCommand(const char *command) {
    if (strcmp(command, "bmov") == 0) {
        printf("Comando bmov valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "rbm"
int validateRbmCommand(const char *command) {
    if (strcmp(command, "rbm") == 0) {
        printf("Comando rbm valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "begin"
int validateBeginCommand(const char *command) {
    if (strcmp(command, "begin") == 0) {
        printf("Comando begin valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "end"
int validateEndCommand(const char *command) {
    if (strcmp(command, "end") == 0) {
        printf("Comando end valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

void validaComandos(char *command){
    // Validar comandos
    if (validateUsersCommand(command)) {
        // Lógica para o comando "users"
    } else if (validateKickCommand(command)) {
        // Lógica para o comando "kick"
    } else if (validateBotsCommand(command)) {
        // Lógica para o comando "bots"
    } else if (validateBmovCommand(command)) {
        // Lógica para o comando "bmov"
    } else if (validateRbmCommand(command)) {
        // Lógica para o comando "rbm"
    } else if (validateBeginCommand(command)) {
        // Lógica para o comando "begin"
    } else if (validateEndCommand(command)) {
        // Lógica para o comando "end"
    } else {
        printf("Comando desconhecido\n");
    }

}

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Por favor insira seu nome");
    }

    recebeLabirinto();

    enviaCredenciais(argv[1]);

    return 0;
}
