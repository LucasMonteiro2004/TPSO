#ifndef MOTOR_H
#define MOTOR_H

#define MAX_BOT_MESSAGES 100

struct BotMessage {
    char content[MAX_COMMAND_LENGTH];
};

struct Coordinates {
    int x;
    int y;
};

struct CommandAdmin {
    char name[MAX_COMMAND_LENGTH];
    // outros parâmetros do comando
};

// Declarar outras estruturas de dados e constantes simbólicas necessárias

#endif