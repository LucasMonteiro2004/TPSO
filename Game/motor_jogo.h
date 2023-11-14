#ifndef MOTOR_H
#define MOTOR_H

#define MAX_BOT_MESSAGES 100
#define MAX_COMMAND_LENGTH 50

#define NUM_PLAYERS 3
#define TAM_NOME 20

char *arquivo = "labirinto1.txt";
char *arquivo1 = "labirinto2.txt";
char *arquivo2 = "labirinto3.txt";

struct BotMessage {
    char content[MAX_COMMAND_LENGTH];
};

struct Bot {
    int x;
    int y;
    int duration;
};

struct CommandAdmin {
    char name[MAX_COMMAND_LENGTH];
    // outros parâmetros do comando
};

struct mensages{
char msg[50];
char msgOwner[50];
};

struct commandLine{
char line[50];
char cmdOwner[50];
};

typedef struct player{
char name[TAM_NOME];
int pid;
}player;


#endif

