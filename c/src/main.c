/*
 * Curso de C - menu principal.
 *
 * Escrito em C de proposito. Um curso de C cujo proprio codigo o aluno pode
 * abrir e ler vale mais do que um que descreve a linguagem a partir de fora,
 * e tudo o que este ficheiro usa esta explicado nos primeiros modulos:
 * arrays, structs, ponteiros para funcao e um ciclo.
 *
 * Compilar:   make
 * Correr:     ./curso-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "licoes.h"
#include "ui.h"

/* Uma entrada do menu: o que se mostra e a funcao que corre.
 *
 * `void (*correr)(void)` e um ponteiro para funcao - uma variavel que guarda
 * o endereco de uma funcao em vez de um numero. E o que permite ter a lista
 * de modulos numa tabela em vez de num `switch` gigante. O modulo 8 explica
 * ponteiros; este e o mesmo conceito aplicado a codigo.
 */
struct Modulo {
    const char *titulo;
    void (*correr)(void);
};

static const struct Modulo MODULOS[] = {
    { "Compilar e imprimir",        licao_01_compilar },
    { "Variaveis e tipos",          licao_02_variaveis },
    { "Ler do teclado",             licao_03_input },
    { "Condicoes",                  licao_04_condicoes },
    { "Ciclos",                     licao_05_ciclos },
    { "Arrays e strings",           licao_06_arrays_strings },
    { "Funcoes",                    licao_07_funcoes },
    { "Ponteiros",                  licao_08_ponteiros },
    { "Memoria: malloc e free",     licao_09_memoria },
    { "Structs",                    licao_10_structs },
};

/* Numero de elementos do array. sizeof do array a dividir pelo sizeof de um
 * elemento. Funciona aqui porque MODULOS e mesmo um array; se fosse um
 * ponteiro daria o tamanho do ponteiro - o modulo 6 volta a este ponto. */
#define TOTAL_MODULOS (int)(sizeof MODULOS / sizeof MODULOS[0])

static void mostrar_menu(void)
{
    limpar_ecra();
    titulo("CURSO DE C - DO ZERO A MEMORIA");

    for (int i = 0; i < TOTAL_MODULOS; i++) {
        printf("   [%2d]  %s\n", i + 1, MODULOS[i].titulo);
    }

    printf("\n   [ 0]  Sair\n");
    separador();
}

int main(void)
{
    char escolha[16];

    for (;;) {
        mostrar_menu();
        printf("\n  Escolhe um modulo: ");
        fflush(stdout);

        /* Fim do input (Ctrl-D, ou o programa a ser alimentado por um
         * ficheiro) conta como sair. Sem isto o programa ficava num ciclo
         * infinito a ler nada. */
        if (!ler_linha(escolha, sizeof escolha)) {
            break;
        }

        if (strcmp(escolha, "0") == 0) {
            break;
        }

        /* atoi devolve 0 para texto que nao seja um numero, e 0 ja significa
         * sair, por isso um "abc" cai no ramo de opcao invalida. */
        int n = atoi(escolha);

        if (n >= 1 && n <= TOTAL_MODULOS) {
            limpar_ecra();
            MODULOS[n - 1].correr();
        } else {
            printf("\n  Opcao invalida.\n");
            pausa();
        }
    }

    printf("\n  Ate a proxima.\n\n");
    return 0;
}
