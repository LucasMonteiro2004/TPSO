#include "header.h"

pthread_t thread[2];
Coordenadas lab;
Player players[TAM_CLIENTES];
pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger o acesso dos players
int activePlayers = 0;

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

PlayerCopy recebeCoodenadas() {
    int fd;
    PlayerCopy playerCopy;

    fd = open(pipeJogoUI, O_RDONLY);

    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        exit(EXIT_FAILURE);
    }

    int bytesRead = read(fd, &playerCopy, sizeof(PlayerCopy));

    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    return playerCopy;
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

    // Atualizar a matriz 'lab.coordenates' com 'X' nas posições ocupadas pelos bots
    lab.coordenates[info.y][info.x] = 'X';

    // Imprimir a tabela
    for (int i = 0; i < MAX_LINHAS; ++i) {
        for (int j = 0; j < MAX_COLUNAS; ++j) {
            if (i == info.y && j == info.x) {
                printf("%d\t", info.duration);
                tabBot[i][j]--;
            } else {
                printf("%d\t", tabBot[i][j]);
                if (tabBot[i][j] > 0)
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
            adicionarValor(bot);
            
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

void* recebeCredenciais(void* args) {
    int fd;
    
    while (1) {
        fd = open(pipeJogoUI, O_RDONLY);

        // Leitura dos dados do pipe
        Player receivedPlayer;
        read(fd, &receivedPlayer, sizeof(Player));

        // Encontrar a próxima posição vazia no array
        int index = 0;
        pthread_mutex_lock(&players_mutex); // Lock mutex before accessing players array
        while (index < TAM_CLIENTES && players[index].pid != 0) {
            index++;
        }

        // Se encontrou uma posição vazia, coloca os dados no array
        if (index < TAM_CLIENTES) {
            players[index] = receivedPlayer;
            activePlayers++;  // Incrementa apenas se um jogador foi adicionado
        } else {
            // Tratar o caso em que o array está cheio
            printf("Aviso: O array de jogadores esta cheio.\n");
            break;  // Interrompe a leitura de novos jogadores
        }

        // Print the complete array of users
        printf("Lista de Utilizadores:\n");
        for (int i = 0; i < TAM_CLIENTES; i++) {
            printf("player %d %s %d\n", i, players[i].name, players[i].pid);
        }

        pthread_mutex_unlock(&players_mutex); // Unlock mutex after accessing players array

        close(fd);
    }
    
    return NULL;
}

int main() {
    // Cria as threads
    if (pthread_create(&thread[0], NULL, lancaBot, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread[1], NULL, recebeCredenciais, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Realiza outras operações enquanto as threads estão em execução
    while (1) {
        enviaLabirinto();

        pthread_mutex_lock(&players_mutex); 
        if (activePlayers == TAM_CLIENTES) {
            pthread_mutex_unlock(&players_mutex); 
            break;
        }
        pthread_mutex_unlock(&players_mutex); 
    }

    if (pthread_join(thread[0], NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(thread[1], NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    unlink(pipeMotor);
    unlink(pipeJogoUI);
    
    return 0;
}