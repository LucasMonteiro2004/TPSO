#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ncurses.h>

#define MAX_BOT_MESSAGES 100
#define MAX_COMMAND_LENGTH 50
#define NUM_PLAYERS 5
#define TAM_NOME 20
#define MAX_LINHAS 15
#define MAX_COLUNAS 40

char user[TAM_NOME];
char *arquivo = "labirinto1.txt";
char *arquivo1 = "labirinto2.txt";
char *arquivo2 = "labirinto3.txt";
char *pipeMotor = "pipe_motor";
char *pipeJogoUI = "pipe_ui";

int linhas, colunas;

struct BotMessage {
    char content[MAX_COMMAND_LENGTH];
};

typedef struct Coordenadas{
    char coordenates[MAX_COLUNAS][MAX_LINHAS];
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
}player;