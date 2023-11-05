#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TAMANHO_MAX 3000

char nome[TAMANHO_MAX]; // Alocando memória para o nome

void recebeNome() {
    // ... (sem alterações)
}

void enviaLabirinto() {
    int fd;
    char *pipeNomeado = "/tmp/meu_pipe";

    // Criar o pipe nomeado, se já não existir
    mkfifo(pipeNomeado, 0666);

    char mensagem_enviada[TAMANHO_MAX];
    int bytesRead = 0;

    // Abrir o arquivo de texto
    FILE *file = fopen("labirinto1.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Limpar o buffer da mensagem
    memset(mensagem_enviada, 0, TAMANHO_MAX);

    // Ler o conteúdo completo do arquivo
    bytesRead = fread(mensagem_enviada, sizeof(char), TAMANHO_MAX - 1, file);
    if (bytesRead <= 0) {
        printf("Erro ao ler do arquivo!\n");
        fclose(file);
        return;
    }

    // Adicionar uma quebra de linha se o arquivo não foi completamente lido
    if (bytesRead < TAMANHO_MAX - 1) {
        mensagem_enviada[bytesRead] = '\n';
    }

    // Abrir o pipe para escrita
    fd = open(pipeNomeado, O_WRONLY);
    if (fd == -1) {
        printf("Erro ao abrir o pipe para escrita!\n");
        fclose(file);
        return;
    }

    // Escrever no pipe
    write(fd, mensagem_enviada, strlen(mensagem_enviada));

    // Fechar o pipe e o arquivo
    close(fd);
    fclose(file);
}

void leComandosAdministrador() {
    char comando[TAMANHO_MAX];
    
    // Simulação da leitura de comandos do administrador
    // Vamos supor um comando "test_bot" para lançar um bot
    printf("Digite um comando do administrador: ");
    scanf("%s", comando);

    // Aqui você pode validar a sintaxe do comando e tomar decisões apropriadas
    if (strcmp(comando, "test_bot") == 0) {
        // Simulação do lançamento do primeiro bot
        printf("Bot lançado. Digite as coordenadas e duração (Exemplo: '3 5 10'): ");
        int x, y, duracao;
        scanf("%d %d %d", &x, &y, &duracao);
        printf("RECEBI: %d %d %d\n", x, y, duracao);
    } else {
        printf("Comando inválido.\n");
    }
}


int main() {
    enviaLabirinto();

    return 0;
}
