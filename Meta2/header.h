#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <ncurses.h>

#define MAX_BOT_MESSAGES 100
#define MAX_COMMAND_LENGTH 50
#define NUM_PLAYERS 5
#define TAM_NOME 20
#define MAX_LINHAS 15
#define MAX_COLUNAS 41
#define TAM_CLIENTES 5

char user[TAM_NOME];
char *arquivo = "labirinto1.txt";
char *arquivo1 = "labirinto2.txt";
char *arquivo2 = "labirinto3.txt";
char *pipeMotor = "pipe_motor";
char *pipeJogoUI = "pipe_ui";

int updateLabirinto = 0;

struct BotMessage {
    char content[MAX_COMMAND_LENGTH];
};

typedef struct Coordenadas{
    char coordenates[MAX_LINHAS][MAX_COLUNAS];
}Coordenadas;

typedef struct Bot {
    int x;
    int y;
    int duration;
}Bot;

struct CommandAdmin {
    char name[MAX_COMMAND_LENGTH];
    // outros parâmetros do comando
};

struct mensages{
char msg[50];
char msgOwner[50];
};

typedef struct commandLine{
char line[50];
char cmdOwner[50];
}commandLine;

typedef struct player{
char name[TAM_NOME];
int pid;
int x;
int y;
char pipeJogador[MAX_COMMAND_LENGTH];
}Player;

typedef struct {
    int playerX;
    int playerY;
    char username[TAM_NOME];
} MiniTerminalData;

typedef struct{
    int playerx, playery;
    char Symbol;
} PlayerCopy;

Player players[TAM_CLIENTES];
Bot b;
int validaComandos(char *command);
int is_obstacle(int x, int y, const char grid[MAX_LINHAS][MAX_COLUNAS]);
int is_Fim(int x, int y, const char grid[MAX_LINHAS][MAX_COLUNAS]);
void recebecoordenates(int playerX, int playerY, char username[TAM_NOME]);
void enviaCredenciais(char nome[TAM_NOME]);
void* lancaBot(void* args);
void obterCoordenadasJogadores(Player players[TAM_CLIENTES]);