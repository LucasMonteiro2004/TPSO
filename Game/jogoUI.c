#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ncurses.h>

#include "motor_jogo.h"

player p;

void recebeLabirinto() {
    int fd;
    char *pipeNomeado = "/tmp/meu_pipe";

    fd = open(pipeNomeado, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        return;
    }

    char mensagem_recebida[TAMANHO_MAX];

    int bytesRead = read(fd, mensagem_recebida, TAMANHO_MAX - 1);
    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return;
    }

    mensagem_recebida[bytesRead] = '\0'; // Adiciona terminação da string

    for (int i = 0; i < bytesRead; i++) {
        if (mensagem_recebida[i] == 'F') {
            mensagem_recebida[i] = ' ';
        }
    }

    // Inicializa o modo ncurses
    initscr();

    // Processamento da mensagem recebida usando ncurses
    printw("%s\n", mensagem_recebida);

    // Adiciona a segunda área
    printw("Pressione ' ' para entrar no terminal de comandos e Enter para sair\n");

    int ch;
    while ((ch = getch()) != 10) { // 10 é o código ASCII para Enter
        if (ch == ' ') {
            char comando[TAM_NOME];
            printw("Você entrou!\n");
            printw("Comando >> ");
            getstr(comando);
            enviaComandos(comando);
        }
    }

    // Finaliza o modo ncurses
    endwin();

    close(fd);
}

void enviaCredenciais(char nome[TAM_NOME]){
    int fd;
    char *pipe = "pipeNome";

    mkfifo(pipe, 0666);

    fd = open(pipe, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para escrita do nome!\n");
        return;
    }

    p.pid = getpid();
    strncpy(p.name, nome, TAM_NOME);

    write(fd, &p, sizeof(p));
    close(fd);
}

void enviaJogadas(){
    int fd;
    char *pipe = "PipeJogadas";

    mkfifo(pipe, 0666);

    int jogada = 0;

    fd = open(pipe, O_WRONLY);
    if(fd == -1){
        printf("Erro ao abrir pipe para escrita de jogadas!\n");
        return;
    }

    initscr();
    keypad(stdscr, TRUE); // Habilita as teclas especiais, como a tecla de espaço

    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
        exit(0);
    }

    if (ch == KEY_RIGHT) {
        jogada = 1;
    } else if (ch == KEY_LEFT) {
        jogada = 2;
    } else if (ch == KEY_UP) {
        jogada = 3;
    } else if (ch == KEY_DOWN) {
        jogada = 4;
    }

    write(fd, &jogada, sizeof(int));

    // Atualiza a tela
    refresh();

    // Aguarde a entrada do usuário antes de encerrar
    getch();

    // Finaliza o modo ncurses
    endwin();

    close(fd);
}

void create_space_comands() {
    initscr(); // Inicializa a biblioteca ncurses
    cbreak();  // Desativa o buffer de linha (leitura de caracteres sem esperar Enter)
    keypad(stdscr, TRUE); // Habilita as teclas especiais, como a tecla de espaço
    noecho(); // Não exibe os caracteres digitados

    // Obtém o tamanho da tela
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Calcula as dimensões da janela
    int window_rows = rows / 2;
    int window_cols = cols / 2;
    int start_row = (rows - window_rows) / 2;
    int start_col = (cols - window_cols) / 2;

    // Cria a janela
    WINDOW *win = newwin(window_rows, window_cols, start_row, start_col);

    // Habilita o reconhecimento de teclas de função e setas
    keypad(win, TRUE);

    // Configura o cursor invisível
    curs_set(0);

    // Preenche a janela com algum conteúdo
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Pressione ESPAÇO para acessar a área, ENTER para sair.");

    // Atualiza a tela
    refresh();

    // Loop principal
    int ch;
    while ((ch = getch()) != '\n') {
        if (ch == ' ') {
            // Se pressionar espaço, faz alguma ação dentro da área
            // Por exemplo, exibe uma mensagem na janela interna
            werase(win); // Limpa a janela
            box(win, 0, 0);
            mvwprintw(win, 1, 1, "Você está dentro da área! Pressione ENTER para sair.");
            wrefresh(win); // Atualiza a janela interna
        }
    }

    // Limpa e desaloca a janela
    werase(win);
    wrefresh(win);
    delwin(win);

    // Finaliza a biblioteca ncurses
    endwin();
}

void enviaComandos(char comando[TAM_NOME]) {
    int fd;
    char *pipe = "PipeComandos";

    mkfifo(pipe, 0666);

    fd = open(pipe, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir pipe para escrita de comandos!\n");
        return;
    }

    int resultadoValidacao = validaComandos(comando);

    if (resultadoValidacao != 1) {
        char desconhecido[3] = "NA\n";  // Adicione uma quebra de linha para melhor formatação
        write(fd, desconhecido, 3);
    } else if (resultadoValidacao == 1) {
        write(fd, comando, TAM_NOME);
    } else {
        printf("impossível enviar comando!!!\n");
    }

    close(fd);
}


// Função para validar o comando "users"
int validateUsersCommand(const char command[TAM_NOME]) {
    if (strcmp(command, "users") == 0) {
        printf("Comando users valido\n");
        return 1; // Comando valido
    }
}

// Função para validar o comando "kick"
int validateKickCommand(const char command[TAM_NOME]) {
    const char *prefix = "kick ";
    if (strncmp(command, prefix, strlen(prefix)) == 0) {
        printf("Comando kick valido para o jogador: %s\n", command + strlen(prefix));
        return 1; // Comando valido
    } 
}

// Função para validar o comando "bots"
int validateBotsCommand(const char command[TAM_NOME]) {
    if (strcmp(command, "bots") == 0) {
        printf("Comando bots valido\n");
        return 1; // Comando valido
    }
}

// Função para validar o comando "bmov"
int validateBmovCommand(const char command[TAM_NOME]) {
    if (strcmp(command, "bmov") == 0) {
        printf("Comando bmov valido\n");
        return 1; // Comando valido
    }
}

// Função para validar o comando "rbm"
int validateRbmCommand(const char command[TAM_NOME]) {
    if (strcmp(command, "rbm") == 0) {
        printf("Comando rbm valido\n");
        return 1; // Comando valido
    }
}

// Função para validar o comando "begin"
int validateBeginCommand(const char command[TAM_NOME]) {
    if (strcmp(command, "begin") == 0) {
        printf("Comando begin valido\n");
        return 1; // Comando valido
    } 
}

// Função para validar o comando "end"
int validateEndCommand(const char command[TAM_NOME]) {
    if (strcmp(command, "end") == 0) {
        printf("Comando end valido\n");
        return 1; // Comando valido
    }
}

int validaComandos(char command[TAM_NOME]){
    // Validar comandos
    if (validateUsersCommand(command) == 1) {
        // Lógica para o comando "users"
        return 1;
    } else if (validateKickCommand(command) == 1) {
        // Lógica para o comando "kick"
        return 1;
    } else if (validateBotsCommand(command) == 1) {
        // Lógica para o comando "bots"
        return 1;
    } else if (validateBmovCommand(command) == 1) {
        // Lógica para o comando "bmov"
        return 1;
    } else if (validateRbmCommand(command) == 1) {
        // Lógica para o comando "rbm"
        return 1;
    } else if (validateBeginCommand(command) == 1) {
        // Lógica para o comando "begin"
        return 1;
    } else if (validateEndCommand(command) == 1) {
        // Lógica para o comando "end"
        return 1;
    } else {
        printf("Comando desconhecido: %s\n", command);
        return 2;
    }

}

void exibeLabirintoJogadas() {
    recebeLabirinto();
    enviaJogadas();
}

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Por favor insira seu nome");
    }

    strcpy(user, argv[1]);

    initscr(); // Inicializa a biblioteca ncurses
    keypad(stdscr, TRUE); // Habilita as teclas especiais, como a tecla de espaço

    enviaCredenciais(user);
    recebeLabirinto();
    enviaJogadas();

    endwin();

    return 0;
}
