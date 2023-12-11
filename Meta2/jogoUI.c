#include "header.h"

Coordenadas lab;
Player p;

// Função para verificar se uma posição contém um obstáculo ('X')
int is_obstacle(int x, int y, const char grid[MAX_LINHAS][MAX_COLUNAS]) {
    if (x < 0 || x >= MAX_COLUNAS || y < 0 || y >= MAX_LINHAS || grid[y][x] == 'X') {
        return 1; // Fora dos limites da grade ou obstáculo ('X')
    }
    return 0;
}

int is_Fim(int x, int y, const char grid[MAX_LINHAS][MAX_COLUNAS]) {
    if (x < 0 || x >= MAX_COLUNAS || y < 0 || y >= MAX_LINHAS) {
        return 0; // Fora dos limites da grade não é o fim
    }
    return grid[y][x] == 'F';
}

void recebecoordenates(int playerX, int playerY, char username[TAM_NOME]) {
    int fd;
    char playerSymbol = username[0];

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
    initscr(); // Inicializa NCurses
    raw(); // entrada de caracteres é passada para o programa imediatamente, sem a necessidade de pressionar Enter
    keypad(stdscr, TRUE);
    noecho(); // desativa a exibição automática dos caracteres digitados pelo usuário.
    curs_set(0); // Oculta o cursor

    enviaCredenciais(username);

    int x, y;
    for (y = 0; y < MAX_LINHAS; y++) {
        for (x = 0; x < MAX_COLUNAS; x++) {
            if (lab.coordenates[y][x] != 'F')
            {
                printw("%c", lab.coordenates[y][x]);
            }else{
                printw(" ");
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
            mvprintw(7, 50, "Parabens! O jogador chegou ao fim!!!");
            refresh();
            break;
        }
    }

    getch();

    // Adiciona a segunda área
    mvprintw(MAX_LINHAS + 1, 0,"Pressione ' ' para entrar no terminal de comandos e Enter para sair\n");

    int ch;
    while ((ch = getch()) != 10) { // 10 é o código ASCII para Enter
        noraw(); // inverso do raw
        echo(); // inverso de noecho
        curs_set(1); // ativa a exibição do cursor
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

void enviaCredenciais(char nome[TAM_NOME]){
    int fd;
    mkfifo(pipeJogoUI, 0644);

    fd = open(pipeJogoUI, O_WRONLY);

    strcpy(p.name, nome);
    p.pid = getpid();

    write(fd, &p, sizeof(p));

    close(fd);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Em falta [NOME]");
    }
    recebecoordenates( 1, 1, argv[1]);
}