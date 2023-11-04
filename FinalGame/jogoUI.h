#ifndef JOGO_UI_H
#define JOGO_UI_H

#define PIPE_NAME_READ "jogo_motor"
#define PIPE_NAME_UI "jogo_ui"
#define GAME_ENGINE_PROCESS_ID "unique_game_engine_identifier"

#include "motor_jogo.h"

struct Credentials {
    char username[20];
};

struct Command {
    char name[MAX_COMMAND_LENGTH];
    char users;
    char kick;
    char bots;
    char bmov;
    char rbm;
    char begin;
    char end;
    // outros parâmetros do comando
};

// Declarar outras estruturas de dados e constantes simbólicas necessárias
void sendPlayerInfoToMotor(const char *playerName);
void receiveMessageFromMotor();
int validateCommand(struct Command *command);

#endif
