/*
 * ui.h - as pecas que todos os modulos usam para desenhar o ecra.
 *
 * Esta separado do resto porque limpar o ecra e ler uma tecla sao as duas
 * unicas coisas neste programa que mudam entre Windows e Linux. Ficando
 * aqui, o resto do curso nao precisa de saber em que sistema esta a correr.
 */

#ifndef UI_H
#define UI_H

/* Limpa o ecra. `cls` no Windows, `clear` no resto. */
void limpar_ecra(void);

/* Espera que o aluno carregue em ENTER. Usa-se entre partes de uma licao,
 * para ninguem ter de ler tres ecras de uma vez. */
void pausa(void);

/* Uma linha inteira de tracos, a toda a largura da caixa. */
void separador(void);

/* Titulo dentro de uma moldura:
 *
 *   +======================================================+
 *   |  MODULO 1 - COMPILAR E IMPRIMIR                      |
 *   +======================================================+
 */
void titulo(const char *texto);

/* Cabecalho de acao dentro de uma licao (PARTE 1, EXERCICIO, ...). */
void seccao(const char *texto);

/* Le uma linha do teclado para `destino`, sem nunca escrever mais do que
 * `tamanho` bytes. Devolve 1 se leu alguma coisa, 0 se o input acabou.
 *
 * Existe porque `scanf("%s")` nao sabe o tamanho do destino e `gets()` foi
 * removido da linguagem por causa disso. O modulo 3 explica porque.
 */
int ler_linha(char *destino, int tamanho);

/* Faz a pergunta e devolve 1 para sim. Aceita s/S/n/N e ENTER como nao. */
int perguntar_sim(const char *pergunta);

#endif
