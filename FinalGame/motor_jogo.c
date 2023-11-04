// motor_jogo.c
#include "motor_jogo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int numMessagesOne;

void waitForCommandsFromUI() {
    int fd_user, fd_ui;
    char commandBuffer[100];

    mkfifo(PIPE_NAME, 0666);  // Pipe para comandos do usuário
    mkfifo(PIPE_NAME_UI, 0666);  // Novo pipe para comandos do jogo UI

    while (1) {
        // Abrir pipe para comandos do usuário
        fd_user = open(PIPE_NAME, O_RDONLY);
        if (fd_user < 0) {
            perror("Erro ao abrir o pipe do usuário");
            exit(1);
        }

        // Abrir pipe para comandos do jogo UI
        fd_ui = open(PIPE_NAME_UI, O_RDONLY);
        if (fd_ui < 0) {
            perror("Erro ao abrir o pipe do jogo UI");
            exit(1);
        }

        // Ler comando enviado pelo jogoUI
        read(fd_ui, commandBuffer, sizeof(commandBuffer));
        close(fd_ui);

        // Processar o comando recebido apenas se for do processo autorizado
        if (strncmp(commandBuffer, GAME_ENGINE_PROCESS_ID, strlen(GAME_ENGINE_PROCESS_ID)) == 0) {
            processUserCommand(commandBuffer);
        }

        // Fechar pipe do usuário
        close(fd_user);
    }
}

void processUserCommand(const char *command) {
    // Implemente a lógica para processar os comandos do usuário recebidos pelo pipe nomeado
    // Aqui, você lidará com os comandos enviados pelo jogoUI
    printf("Comando recebido do jogoUI: %s\n", command);
    // Realize a lógica do jogo com base no comando recebido
    // Por exemplo, movimento do jogador, ação no jogo, etc.
}

// Função para verificar se uma posição contém um obstáculo ('X')
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

int validateAdminCommand(struct CommandAdmin *adminCommand) {
    // Lógica para validar os comandos do administrador
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo do labirinto>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo do labirinto.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *grid = (char *)malloc(file_size + 1);
    if (grid == NULL) {
        printf("Erro de alocação de memória.\n");
        fclose(file);
        return 1;
    }

    // Lê o labirinto do arquivo txt
    fread(grid, file_size, 1, file);
    grid[file_size] = '\0';
    fclose(file);

    // Envia o labirinto para o jogoUI
    int fd_ui;
    mkfifo(PIPE_NAME_UI, 0666); // Pipe para comandos do jogo UI

    fd_ui = open(PIPE_NAME_UI, O_WRONLY);
    if (fd_ui < 0) {
        perror("Erro ao abrir o pipe do jogo UI");
        exit(1);
    }

    // Envia o labirinto para o jogoUI
    write(fd_ui, grid, strlen(grid) + 1);
    close(fd_ui);

    struct CommandAdmin adminCommand;
    // Lógica para ler e validar comandos do administrador
    while (1) {
        // Ler comando e validar
        // Exemplo de como chamar a função de validação:
        if (validateAdminCommand(&adminCommand)) {
            // Comando válido
            if (strcmp(adminCommand.name, "test_bot") == 0) {
                launchBot();
                struct BotMessage messages[MAX_BOT_MESSAGES];
                // Receber mensagens do bot
                // Mostrar mensagens na tela
                showBotMessages(messages, numMessagesOne);
            }
            // Outros comandos administrativos...
        } else {
            // Comando inválido
        }
    }

    free(grid);

    return 0;
}
