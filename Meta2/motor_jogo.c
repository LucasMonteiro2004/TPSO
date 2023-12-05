#include "header.h"

Coordenadas lab;

Player players[TAM_CLIENTES];

void enviaLabirinto() {
    int fd;
    mkfifo(pipeMotor, 0666);

    fd = open(pipeMotor, O_WRONLY);

    FILE *file = fopen(arquivo, "rt");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    for (int y = 0; y < MAX_LINHAS; y++) {
        for (int x = 0; x < MAX_COLUNAS; x++) {
            fscanf(file, "%c", &lab.coordenates[y][x]);
        }
    }

    write(fd, &lab, sizeof(Coordenadas));

    close(fd);
    fclose(file);
}

void lancaBot(){
    pid_t pid = fork();

    if (pid == -1) {
        // Ocorreu um erro ao criar o processo
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Processo pai
        printf("Processo pai: Esperando pelo término do processo filho...\n");

        // Aguarda o término do processo filho
        waitpid(pid, NULL, 0);

        printf("Processo pai: Processo filho terminou.\n");
    } else {
        // Processo filho
        printf("Processo filho: Executando o programa filho...\n");

        // Redireciona a saída padrão para um arquivo
        int novo_descritor = creat("saida.txt", 0644);
        dup(novo_descritor);
        close(novo_descritor);

        // Substitua "seu_programa" pelo nome do programa que deseja executar
        execlp("./bot", "./bot", "2", "3", (char *)NULL);

        // Se o execlp() falhar, exibe uma mensagem de erro
        perror("execlp");
        exit(EXIT_FAILURE);
    }
}


// Função para validar o comando "users"
int validateUsersCommand(const char *command) {
    if (strcmp(command, "users") == 0) {
        printf("Comando users valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "kick"
int validateKickCommand(const char *command) {
    const char *prefix = "kick ";
    if (strncmp(command, prefix, strlen(prefix)) == 0) {
        printf("Comando kick valido para o jogador: %s\n", command + strlen(prefix));
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "bots"
int validateBotsCommand(const char *command) {
    if (strcmp(command, "bots") == 0) {
        printf("Comando bots valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "bmov"
int validateBmovCommand(const char *command) {
    if (strcmp(command, "bmov") == 0) {
        printf("Comando bmov valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "rbm"
int validateRbmCommand(const char *command) {
    if (strcmp(command, "rbm") == 0) {
        printf("Comando rbm valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "begin"
int validateBeginCommand(const char *command) {
    if (strcmp(command, "begin") == 0) {
        printf("Comando begin valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

// Função para validar o comando "end"
int validateEndCommand(const char *command) {
    if (strcmp(command, "end") == 0) {
        printf("Comando end valido\n");
        return 1; // Comando valido
    } else {
        printf("Comando invalido\n");
        return 0; // Comando invalido
    }
}

int validaComandos(char *command){
    // Validar comandos
    if (validateUsersCommand(command)) {
        // Lógica para o comando "users"
    } else if (validateKickCommand(command)) {
        // Lógica para o comando "kick"
    } else if (validateBotsCommand(command)) {
        // Lógica para o comando "bots"
    } else if (validateBmovCommand(command)) {
        // Lógica para o comando "bmov"
    } else if (validateRbmCommand(command)) {
        // Lógica para o comando "rbm"
    } else if (validateBeginCommand(command)) {
        // Lógica para o comando "begin"
    } else if (validateEndCommand(command)) {
        // Lógica para o comando "end"
    } else {
        printf("Comando desconhecido\n");
    }

}

void recebeCredenciais(){
    int fd;

    fd = open(pipeJogoUI, O_RDONLY);

    // Leitura dos dados do pipe
    Player receivedPlayer;
    read(fd, &receivedPlayer, sizeof(Player));

    // Encontrar a próxima posição vazia no array
    int index = 0;
    while (index < TAM_CLIENTES && players[index].pid != 0) {
        index++;
    }

    // Se encontrou uma posição vazia, coloca os dados no array
    if (index < TAM_CLIENTES) {
        players[index] = receivedPlayer;
    } else {
        // Tratar o caso em que o array está cheio
        printf("Erro: O array de players está cheio.\n");
    }

    for (int i = 0; i < TAM_CLIENTES; i++)
    {
        printf("player %d %s %d\n", i, (players)[i].name, (players)[i].pid);
    }
    
    close(fd);
}

int main(){
    enviaLabirinto();
    recebeCredenciais();
    lancaBot();
}