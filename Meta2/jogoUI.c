#include "header.h"

Coordenadas lab;

// Função para verificar se uma posição contém um obstáculo ('X')
int is_obstacle(int x, int y, const char *grid) {
    if (x < 0 || x >= MAX_COLUNAS || y < 0 || y >= MAX_LINHAS) {
        return 1; // Fora dos limites da grade é considerado um obstáculo
    }
    return grid[y * MAX_COLUNAS + x] == '#';
}

int is_Fim(int x, int y, const char *grid) {
    if (x < 0 || x >= MAX_COLUNAS || y < 0 || y >= MAX_LINHAS) {
        return 0; // Fora dos limites da grade não é o fim
    }
    return grid[y * MAX_COLUNAS + x] == 'F';
}

void recebecoordenates(int playerX, int playerY, char playerSymbol) {
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

    // Inicializa o modo ncurses
    // Inicializa o modo ncurses
    initscr(); // Inicializa NCurses
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0); // Oculta o cursor

    int x, y;
    for (y = 0; y < MAX_LINHAS; y++) {
        for (x = 0; x < MAX_COLUNAS; x++) {
            if (lab.coordenates[y][x] == 'X') {
                mvaddch(y, x, 'X');
            } else if (lab.coordenates[y][x] == ' ') {
                mvaddch(y, x, ' ');
            }
        }
    }

    refresh(); // Atualiza a tela

    while (1) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            break;
        }

        int new_x = playerX;
        int new_y = playerY;

        if (ch == KEY_RIGHT) {
            new_x++;
        } else if (ch == KEY_LEFT) {
            new_x--;
        } else if (ch == KEY_UP) {
            new_y--;
        } else if (ch == KEY_DOWN) {
            new_y++;
        }

        if (!is_obstacle(new_x, new_y, lab.coordenates)) {
            mvaddch(playerY, playerX, ' '); // Apaga a posição anterior do jogador
            playerX = new_x;
            playerY = new_y;
            mvaddch(playerY, playerX, playerSymbol); // Desenha o jogador na nova posição
            refresh();
        }

        if (is_Fim(playerX, playerY, lab.coordenates)) {
            mvprintw(MAX_LINHAS + 1, 0, "Parabens! O jogador chegou ao fim!!!");
            refresh();
            break;
        }
    }

    getch();

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
    recebecoordenates( 1, 1, 'L');
}