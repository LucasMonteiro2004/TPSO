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

int main(){
    enviaLabirinto();
    lancaBot();
}