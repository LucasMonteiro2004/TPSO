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

char linha[80];
int GRID_WIDTH = 0, GRID_HEIGHT = 0;

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


void executaBot(const char *program_path, char *const arguments[]) {
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
        dup2(pipe_fd[1], STDOUT_FILENO);

        // Fecha a extremidade de escrita do pipe
        close(pipe_fd[1]);

        execvp(program_path, arguments);

        perror("Erro ao executar o programa bot");
        exit(EXIT_FAILURE);
    } else {
        // Fecha a extremidade de escrita do pipe
        close(pipe_fd[1]);

        FILE *pipe_stream = fdopen(pipe_fd[0], "r");
        if (pipe_stream == NULL) {
            perror("Erro ao abrir o fluxo do pipe");
            exit(EXIT_FAILURE);
        }

        fscanf(pipe_stream, "%d %d %d", &bot.x, &bot.y, &bot.duration);

        fclose(pipe_stream);

        // Aguarda o término do processo filho
        wait(NULL);

        printf("RECEBI do bot: %d %d %d\n", bot.x, bot.y, bot.duration);
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
    char *arguments[] = {"./bot", "2", "3", NULL};
    
    inicializa();
    enviaLabirinto();
    recebeComandos();
    NomeUtilizador();
    executaBot("./bot", arguments);
    recebeJogada();
    return 0;
}