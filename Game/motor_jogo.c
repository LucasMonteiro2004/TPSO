#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "motor_jogo.h"

player p;
Bot bot;
Coordenadas coor;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void enviaLabirinto() {
    int fd;
    char *pipeNomeado = "/tmp/meu_pipe"; // Nome do pipe para comunicação

    mkfifo(pipeNomeado, 0666); // Criação do pipe

    char mensagem_enviada[MAX_LINHAS][MAX_COLUNAS + 1]; // +1 para o caractere nulo

    FILE *file = fopen(arquivo, "rt");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Leitura do conteúdo do arquivo
    for (int i = 0; i < MAX_LINHAS; i++) {
        if (fgets(mensagem_enviada[i], MAX_COLUNAS + 1, file) == NULL) {
            printf("Erro ao ler o labirinto do arquivo!\n");
            fclose(file);
            return;
        }
    }

    fd = open(pipeNomeado, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para escrita!\n");
        fclose(file);
        return;
    }

    // Escreve no pipe
    for (int i = 0; i < MAX_LINHAS; i++) {
        write(fd, mensagem_enviada[i], strlen(mensagem_enviada[i]));
    }

    close(fd);
    fclose(file);
}

void NomeUtilizador() {
    int fd;
    char *pipe = "pipeNome";

    if (access(pipe, F_OK) != 0) {
        printf("[ERRO] FIFO '%s' nao existe", pipe);
        return;
    }

    fd = open(pipe, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        return;
    }

    

    int bytesRead = read(fd, &p, sizeof(player));
    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return;
    }


    printf("Nome: %s\nPID: %d\n", p.name, p.pid);

    close(fd);
}

int is_obstacle(int x, int y, const char *grid) {
    if (x < 0 || x >= coor.x || y < 0 || y >= coor.y) {
        return 1; // Fora dos limites da grade é considerado um obstáculo
    }
    return grid[y * coor.x + x] == 'X';
}

int is_Fim(int x, int y, const char *grid) {
    if (x < 0 || x >= coor.x || y < 0 || y >= coor.y) {
        return 0; // Fora dos limites da grade não é o fim
    }
    return grid[y * coor.x + x] == 'F';
}

void inicializa(){
    printf("\t\t\tMOTOR DE JOGO\n");
    printf("============================================================\n");
    printf("============================================================\n");
}

player* criarArray(int tamanho) {
    player* array = (player*)malloc(tamanho * sizeof(player));

    if (array == NULL) {
        printf("Erro ao alocar memória para o array.\n");
        return NULL;
    }

    for (int i = 0; i < tamanho; i++) {
        array[i].pid = -1;
        strcpy(array[i].name, "");
    }

    // Retorna o ponteiro para o array alocado
    return array;
}

int recebeJogada() {
    int fd;
    char *pipe = "PipeJogadas";

    int jogada = 0;

    fd = open(pipe, O_RDONLY);
    if(fd == -1){
        printf("Erro ao abrir pipe para leitura de jogadas!\n");
        return 0;
    }

    read(fd, &jogada, sizeof(int));
    close(fd);

    printf("jogada %d\n", jogada);

    return jogada;
}


int processaJogada(int *playerX, int *playerY, int jogada, const char *labirinto) {
    int new_x = *playerX;
    int new_y = *playerY;

    if (jogada == 1 && labirinto[new_y * coor.x + new_x + 1] != 'X') {
        new_x++;
    } else if (jogada == 2 && labirinto[new_y * coor.x + new_x - 1] != 'X') {
        new_x--;
    } else if (jogada == 3 && labirinto[(new_y - 1) * coor.x + new_x] != 'X') {
        new_y--;
    } else if (jogada == 4 && labirinto[(new_y + 1) * coor.x + new_x] != 'X') {
        new_y++;
    }

    // Verifica se o jogador atingiu o objetivo
    if (is_Fim(new_x, new_y, labirinto)) {
        printf("Parabens! Jogador atingiu o objetivo!\n");
        return 0; // Sinaliza que o jogo deve encerrar
    }

    *playerX = new_x;
    *playerY = new_y;

    return 1; // Sinaliza que o jogo deve continuar
}

void jogoLoop(int playerX, int playerY, const char *labirinto) {
    int jogada;

    do {
        jogada = recebeJogada();
    } while (processaJogada(&playerX, &playerY, jogada, labirinto));
}


void executaBot(const char *program_path) {
    pid_t pid;
    struct Bot bot;
    int pipe_fd[2];

    // Cria um pipe para comunicação entre processos
    if (pipe(pipe_fd) == -1) {
        perror("Erro ao criar pipe");
        exit(EXIT_FAILURE);
    }

    // Cria um novo processo
    if ((pid = fork()) == -1) {
        perror("Erro ao criar processo");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Fecha a extremidade de leitura do pipe
        close(pipe_fd[0]);

        // Redireciona a saída padrão para a extremidade de escrita do pipe
        if (dup(pipe_fd[1]) == -1) {
            perror("Erro ao redirecionar saída padrão para o pipe");
            exit(EXIT_FAILURE);
        }

        // Fecha a extremidade de escrita do pipe
        close(pipe_fd[1]);

        execl(program_path, "./bot","2", "3", NULL);

        perror("Erro ao executar o programa bot");
        exit(EXIT_FAILURE);
    } else {
        // Fecha a extremidade de escrita do pipe
        close(pipe_fd[1]);

        read(pipe_fd[0], &bot, sizeof(bot));

        // Aguarda o término do processo filho
        wait(NULL);
    }
}

void recebeComandos(){
    int fd;
    char *pipe = "PipeComandos";

    char comando[TAM_NOME];

    fd = open(pipe, O_RDONLY);
    if(fd == -1){
        printf("Erro ao abrir pipe para leitura de Comandos!\n");
        return;
    }

    read(fd, &comando, TAM_NOME);
    close(fd);

    printf("Comando >> %s\n", comando);
}

int main(int argc, char *argv[]) {
    inicializa();
    NomeUtilizador();
    enviaLabirinto();
    recebeComandos();
    executaBot("./bot");
    recebeJogada();
    return 0;
}