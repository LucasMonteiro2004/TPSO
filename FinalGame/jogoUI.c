#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#include "jogoUI.h"

#define GRID_WIDTH 80
#define GRID_HEIGHT 30

void sendCommandToGameEngine(const char *command) {
    int fd;
    char formattedCommand[100];

    mkfifo(PIPE_NAME_UI, 0666);  // Pipe para comandos do jogo UI

    // Formatar o comando com o identificador do processo do motor do jogo
    sprintf(formattedCommand, "%s%s", GAME_ENGINE_PROCESS_ID, command);

    // Abrir o pipe para escrita
    fd = open(PIPE_NAME_UI, O_WRONLY);
    if (fd < 0) {
        perror("Erro ao abrir o pipe do jogo UI");
        exit(1);
    }

    // Escrever o comando no pipe
    write(fd, formattedCommand, strlen(formattedCommand) + 1);
    close(fd);
}

void sendPlayerInfoToMotor(const char *playerName) {
    int fd;
    char playerInfo[50];

    // Construindo a mensagem a ser enviada para o motor
    sprintf(playerInfo, "PLAYER_INFO:%s", playerName);

    // Criando o pipe nomeado (FIFO) se não existir
    mkfifo(PIPE_NAME, 0666);

    // Abrindo o pipe para escrita
    fd = open(PIPE_NAME, O_WRONLY);
    if (fd < 0) {
        perror("Erro ao abrir o pipe");
        exit(1);
    }

    // Escrevendo as informações do jogador no pipe
    write(fd, playerInfo, strlen(playerInfo) + 1);
    close(fd);
}

void receiveMessageFromMotor() {
    int fd;
    char message[100];

    // Abrindo o pipe para leitura
    fd = open(PIPE_NAME_READ, O_RDONLY);
    if (fd < 0) {
        perror("Erro ao abrir o pipe");
        exit(1);
    }

    // Lendo a mensagem do pipe
    read(fd, message, sizeof(message));
    printf("Mensagem recebida do motor: %s\n", message);
    close(fd);
}

int validateCommand(struct Command *command) {
    // Lógica para validar os comandos
}

void displayGrid(const char *grid) {
    initscr();
    int x, y;
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++) {
        y = i / GRID_WIDTH;
        x = i % GRID_WIDTH;

        mvaddch(y, x, grid[i]);
    }
    refresh();
    getch(); // Aguarda um caractere para encerrar a exibição
    endwin();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <Nome>\n", argv[0]);
        return 1;
    }

    char * playerName = argv[1];

    sendPlayerInfoToMotor(playerName);

    int fd_ui;
    char receivedGrid[GRID_WIDTH * GRID_HEIGHT + 1];

    // Aguarda sinal do motor para iniciar a execução
    fd_ui = open(PIPE_NAME_UI, O_RDONLY);
    if (fd_ui < 0) {
        perror("Erro ao abrir o pipe do jogo UI");
        exit(1);
    }

    // Aguarda e recebe o labirinto do motor
    read(fd_ui, receivedGrid, sizeof(receivedGrid));
    close(fd_ui);

    struct Credentials userCredentials;
    getCredentials(&userCredentials);

    struct Command userCommand;
    // Lógica para ler comandos do utilizador e validá-los
    while (1) {
        // Ler comando e validar
        // Exemplo de como chamar a função de validação:
        if (validateCommand(&userCommand)) {
            // Comando válido
        } else {
            // Comando inválido
        }
    }

    displayGrid(receivedGrid);

    return 0;
}
