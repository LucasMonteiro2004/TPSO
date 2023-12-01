#include "header.h"

Coordenadas lab;

void enviaLabirinto() {
    int fd;
    mkfifo(pipeMotor, 0666);

    fd = open(pipeMotor, O_WRONLY);

    FILE *file = fopen(arquivo, "rt");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    for (int i = 0; i < MAX_LINHAS; i++) {
        for (int j = 0; j < MAX_COLUNAS; j++) {
            fscanf(file, " %c", &lab.coordenates[i][j]);
        }
    }

    write(fd, &lab, sizeof(Coordenadas));

    close(fd);
    fclose(file);
}

int main(){
    enviaLabirinto();
}