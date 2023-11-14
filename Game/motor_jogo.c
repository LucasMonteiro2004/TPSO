#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "motor_jogo.h"

#define TAMANHO_MAX 3000

player p;

char linha[80];
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

    //p.name = strdup(nome);

    printf("Nome: %s\nPID: %d\n", p.name, p.pid);
    //free(p.name);

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

/*
// inicializa o arrayJogadas
player* incializiaJogadas(int numPlayers){
player *players;
players= (player *) malloc(numPlayers);
return players;
}
// Atualiza o tamanho Array de jogadas caso um Jogador saia do jogo
player* atualizaJogadas(int numPlayers,char pidOut, player *players)
{  
   player* newPlayers;
   newPlayers = (player *) malloc(numPlayers);
   
   if(pidOut=players[numPlayers-1].pid)
   {
    players[numPlayers] = NULL;
    for(int i=0;i<numPlayers-1;i++)
    newPlayers[i]=players[i];
   } 
   
   for(int i=0;i<numPlayers-1;i++)
   {  newPlayers[i]=players[i];
    if(pidOut==players[i].pid)        
   {  
      for(int j=i;j<numPlayers-1;j++)
          newPlayers[i]=players[i+1];
        
        }
        
   }
   
return  newPlayers;
}
*/
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

    return jogada;
}


int processaJogada(int *playerX, int *playerY, int jogada, const char *labirinto) {
    int new_x = *playerX;
    int new_y = *playerY;

    if (jogada == 1 && labirinto[new_y * GRID_WIDTH + new_x + 1] != 'X') {
        new_x++;
    } else if (jogada == 2 && labirinto[new_y * GRID_WIDTH + new_x - 1] != 'X') {
        new_x--;
    } else if (jogada == 3 && labirinto[(new_y - 1) * GRID_WIDTH + new_x] != 'X') {
        new_y--;
    } else if (jogada == 4 && labirinto[(new_y + 1) * GRID_WIDTH + new_x] != 'X') {
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

void executaBot(const char *program_path, char *const arguments[]){
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // Este é o processo filho
        // Fecha a extremidade de leitura do pipe
        close(pipe_fd[0]);

        // Redireciona a saída padrão para o pipe
        dup2(pipe_fd[1], STDOUT_FILENO);

        // Fecha a extremidade de escrita do pipe, pois não a usaremos
        close(pipe_fd[1]);

        // Este é o ponto onde o programa é executado
        execvp(program_path, arguments);

        // Se execvp falhar, imprime uma mensagem de erro
        perror("execvp");
        // Termina o processo filho
        _exit(EXIT_FAILURE);
    } else {
        // Este é o processo pai
        // Fecha a extremidade de escrita do pipe
        close(pipe_fd[1]);

        // Leitura e impressão da saída do programa
        char buffer[1024];
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            // Adiciona a mensagem "Recebi do bot: " antes de cada linha da saída
            printf("Recebi do bot: %.*s", (int)bytes_read, buffer);
        }

        // Aguarda o término do processo filho
        int status;
        waitpid(pid, &status, 0);

        // Fecha a extremidade de leitura do pipe
        close(pipe_fd[0]);

        if (WIFEXITED(status)) {
            printf("O programa foi executado com código de saída: %d\n", WEXITSTATUS(status));
        } else {
            printf("O programa terminou de forma anormal.\n");
        }
    }
}

int main(int argc, char *argv[]) {
    const char *program_path = "./bot";  // Substitua pelo caminho real do seu programa
    char *const arguments[] = {program_path, 4, 5, NULL};

    inicializa();
    enviaLabirinto();
    NomeUtilizador();
    executaBot(program_path, arguments);
    return 0;
}

/*
fazer com que o pai seja quem executa o motor de jogo e os filhos os demais -> emilio
estrutura user com nome e pid -> emilio
ter um array dinamico que guarda a estrutura user -> emilio
estrutura para comandos -> emilio
estrutura de mensagens -> emilio
logica de movimentação de usuario -> lucas
*/
