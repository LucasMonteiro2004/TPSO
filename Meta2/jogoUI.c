#include "header.h"

void recebeLabirinto() {
    int fd;

    fd = open(pipeMotor, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        return;
    }

    char lab[MAX_LINHAS][MAX_COLUNAS + 1]; 

    int bytesRead = read(fd, lab, sizeof(lab));
    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return;
    }

    // Inicializa o modo ncurses
    initscr();

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printw("%d ", lab[i][j]);
        }
        printw("\n");
    }

    // Adiciona a segunda área
    printw("Pressione ' ' para entrar no terminal de comandos e Enter para sair\n");

    int ch;
    while ((ch = getch()) != 10) { // 10 é o código ASCII para Enter
        if (ch == ' ') {
            char comando[TAM_NOME];
            printw("Você entrou!\n");
            printw("Comando >> ");
            getstr(comando);
        }
    }

    // Finaliza o modo ncurses
    endwin();

    close(fd);
}