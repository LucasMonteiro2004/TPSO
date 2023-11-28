#include "header.h"

Coordenadas lab;

void recebeLabirinto() {
    int fd;

    fd = open(pipeMotor, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        return;
    }

    int bytesRead = 0;

    bytesRead = read(fd, &lab, sizeof(Coordenadas));

    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return;
    }

    for (int i = 0; i < MAX_LINHAS; i++) {
        for (int j = 0; i < MAX_COLUNAS; i++)
        {
            if(lab.coordenates[i][j] == 'F') {
            lab.coordenates[i][j] = ' ';
            }
        }
    }

    // Inicializa o modo ncurses
    initscr();

    for (int i = 0; i < MAX_LINHAS; i++) {
        for (int j = 0; j < MAX_COLUNAS; j++) {
            printw("%c ", lab.coordenates[i][j]);
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

int main(){
    recebeLabirinto();
}