#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAMANHO_MAX 3000

char *username;

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

    // Processamento da mensagem recebida
    printf("%s\n", mensagem_recebida);

    close(fd);
}

void recebeNome(char *nome){
    int fd;
    char *pipe = "pipeNome";

    mkfifo(pipe, 0666);

    fd = open(pipe, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para escrita!\n");
        return;
    }

    write(fd, nome, strlen(nome));
    close(fd);
}


int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("Por favor insira seu nome");
    }

    recebeLabirinto();

    strcpy(username, argv[1]);

    recebeNome(username);

    return 0;
}
