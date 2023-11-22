#ifndef MOTOR_H
#define MOTOR_H

#define MAX_BOT_MESSAGES 100
#define MAX_COMMAND_LENGTH 50

#define NUM_PLAYERS 3
#define TAM_NOME 20
#define TAMANHO_MAX 3000
#define MAX_LINHAS 15
#define MAX_COLUNAS 40

#include <stdio.h>

char user[TAM_NOME];
char *arquivo = "labirinto1.txt";
char *arquivo1 = "labirinto2.txt";
char *arquivo2 = "labirinto3.txt";


struct BotMessage {
    char content[MAX_COMMAND_LENGTH];
};

typedef struct Coordenadas{
    char coordenates[MAX_LINHAS][MAX_COLUNAS];
    int x, y;
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

void recebeLabirinto();
void enviaCredenciais(char nome[TAM_NOME]);
void enviaJogadas();
void create_space_comands();
void enviaComandos(char comando[TAM_NOME]);
int validateUsersCommand(const char command[TAM_NOME]);
int validateKickCommand(const char command[TAM_NOME]);
int validateBotsCommand(const char command[TAM_NOME]);
int validateBmovCommand(const char command[TAM_NOME]);
int validateRbmCommand(const char command[TAM_NOME]);
int validateBeginCommand(const char command[TAM_NOME]);
int validaComandos(char command[TAM_NOME]);
void exibeLabirintoJogadas();

#endif