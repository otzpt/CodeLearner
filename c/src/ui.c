#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ui.h"

/* Largura interior da moldura. Mudar aqui muda todas as caixas do curso. */
#define LARGURA 54

void limpar_ecra(void)
{
#ifdef _WIN32
    /* O cmd.exe antigo nao percebe codigos ANSI, e o cls trata disso. */
    system("cls");
#else
    /* Codigos ANSI em vez de system("clear"): nao lanca outro processo e
     * nao depende da variavel TERM, que falta quando o programa corre fora
     * de um terminal normal. Sem isto aparece "TERM environment variable
     * not set" a cada ecra.
     *
     *   [H   poe o cursor no canto superior esquerdo
     *   [2J  limpa o ecra visivel
     *   [3J  limpa tambem o historico que ficou acima
     */
    fputs("\033[H\033[2J\033[3J", stdout);
    fflush(stdout);
#endif
}

void pausa(void)
{
    printf("\n  Carrega em ENTER para continuar...");
    fflush(stdout);

    /* Consome ate ao fim da linha. Sem isto, um ENTER deixado para tras por
     * uma leitura anterior faria a pausa passar sozinha. */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descartar */
    }
}

void separador(void)
{
    printf("  ");
    for (int i = 0; i < LARGURA; i++) {
        putchar('-');
    }
    putchar('\n');
}

/* Escreve `texto` seguido de espacos ate encher a largura da moldura. */
static void linha_preenchida(const char *texto, char borda)
{
    int escritos = (int) strlen(texto);
    printf("  %c %s", borda, texto);

    for (int i = escritos; i < LARGURA - 3; i++) {
        putchar(' ');
    }
    printf("%c\n", borda);
}

static void moldura(char canto)
{
    printf("  +");
    for (int i = 0; i < LARGURA - 2; i++) {
        putchar(canto);
    }
    printf("+\n");
}

void titulo(const char *texto)
{
    putchar('\n');
    moldura('=');
    linha_preenchida(texto, '|');
    moldura('=');
    putchar('\n');
}

void seccao(const char *texto)
{
    printf("\n  %s\n", texto);
    separador();
}

int ler_linha(char *destino, int tamanho)
{
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return 0;
    }

    /* fgets guarda o '\n' final; o resto do programa nao o quer. */
    destino[strcspn(destino, "\n")] = '\0';
    return 1;
}

int perguntar_sim(const char *pergunta)
{
    char resposta[8];

    printf("\n  %s (s/N): ", pergunta);
    fflush(stdout);

    if (!ler_linha(resposta, sizeof resposta)) {
        return 0;
    }
    return resposta[0] == 's' || resposta[0] == 'S';
}
