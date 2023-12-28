#include "header.h"

pthread_t thread[2];
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

void extractBotData(const char *str, Bot *bot) {
    if (sscanf(str, "%d %d %d", &bot->x, &bot->y, &bot->duration) != 3) {
        fprintf(stderr, "Falha ao extrair dados do Bot: formato incorreto.\n");
    }
}
void passaTabela(int tab[MAX_LINHAS][MAX_COLUNAS])
{
    for(int i=0;i<MAX_LINHAS;i++)
    {
        for(int j=0;j<MAX_COLUNAS;j++)
        {
            if(tab[i][j]>0)
                lab.coordenates[i][j]='X';
        }
    }
}
void adicionarValor(Bot info) {
    static int tabBot[MAX_LINHAS][MAX_COLUNAS] = {{0}};

    // Adicionar o valor à tabela
    tabBot[info.y][info.x] = info.duration;

    // Imprimir a tabela
    for (int i = 0; i < MAX_LINHAS; ++i) {
        for (int j = 0; j < MAX_COLUNAS; ++j) {
            if (i == info.y && j == info.x) {
                printf("%d\t",info.duration);
                tabBot[i][j]--;
            } else {
                printf("%d\t", tabBot[i][j]);
                if(tabBot[i][j]>0)
                --tabBot[i][j];
            }
        }
        printf("\n");
    }
    passaTabela(tabBot);
}


void* lancaBot(void* args) {
    int pipefd[2];
    pid_t pid;
    char buffer[1024];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Processo pai
        close(pipefd[1]);  // Fecha a extremidade de escrita do pipe
        ssize_t num_read = read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);

        if (num_read > 0) {
            Bot bot;
            extractBotData(buffer, &bot);
            //adicionarValor(bot);
            
            // Exibindo os valores da estrutura
            printf("Bot x: %d\n", bot.x);
            printf("Bot y: %d\n", bot.y);
            printf("Bot duration: %d\n", bot.duration);
        } else {
            fprintf(stderr, "Falha ao ler do pipe.\n");
        }
    } else {
        // Processo filho
        close(pipefd[0]);  // Fecha a extremidade de leitura do pipe
        dup(pipefd[1]);    // Redireciona a saída padrão para o pipe
        close(pipefd[1]);

        // Substitua abaixo pela chamada ao seu programa com os argumentos apropriados
        execlp("./bot", "./bot", "2", "3", (char *)NULL);

        perror("execlp");
        exit(EXIT_FAILURE);
    }

    return NULL;
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

int main() {
    // Cria a thread
    if (pthread_create(&thread[0], NULL, lancaBot, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Realiza outras operações enquanto a thread lancaBot está em execução
    while (1)
    {
        enviaLabirinto();
        recebeCredenciais();
    }

    // Aguarda a conclusão da thread lancaBot
    if (pthread_join(thread[0], NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    unlink(pipeMotor);
    unlink(pipeJogoUI);
    return 0;
}
