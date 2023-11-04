#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAMANHO_MAX 3000

int main() {
    int fd;
    char *pipeNomeado = "/tmp/meu_pipe";

    // Abrir o pipe para leitura
    fd = open(pipeNomeado, O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para leitura!\n");
        return 1;
    }

    char mensagem_recebida[TAMANHO_MAX];

    // Ler dados do pipe
    int bytesRead = read(fd, mensagem_recebida, TAMANHO_MAX);
    if (bytesRead == -1) {
        printf("Erro ao ler do pipe!\n");
        close(fd);
        return 1;
    }

    // Fechar o pipe
    close(fd);

    // Substituir as letras 'F' por espaços
    for (int i = 0; i < bytesRead; i++) {
        if (mensagem_recebida[i] == 'F') {
            mensagem_recebida[i] = ' ';
        }
    }

    // Imprimir os dados recebidos com substituição das letras 'F' por espaços
    printf("Conteúdo recebido, substituindo as letras 'F' por espaços:\n");
    printf("%s\n", mensagem_recebida);

    return 0;
}
