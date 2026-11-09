/*
 * Portuguese (PT-PT) text for the menus, prompts and module summaries.
 * CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings_pt.h"

static int pt_mode = -1;

int lang_pt(void)
{
    if (pt_mode < 0) {
        const char *value = getenv("CODELEARNER_LANG");
        pt_mode = value != NULL && strcmp(value, "pt-pt") == 0;
    }
    return pt_mode;
}

static const char *const TEXT[][2] = {
    { "Press ENTER to continue...", "Prime ENTER para continuar..." },
    { "Your answer:", "A tua resposta:" },
    { "CORRECT.", "CERTO." },
    { "NOT QUITE. The answer is:", "NÃO É BEM ISSO. A resposta é:" },
    { ">> EXERCISE - MODULE", ">> EXERCÍCIO - MÓDULO" },
    { ">> WRITE THIS YOURSELF, in a real file", ">> ESCREVE ISTO TU, num ficheiro a sério" },
    { ">> TRY THIS YOURSELF, in a real directory", ">> EXPERIMENTA TU, numa pasta a sério" },
    { "Try it with this input:", "Experimenta com esta entrada:" },
    { "It must print:", "Tem de escrever:" },
    { "You should see:", "Deves ver:" },
    { "The result must be:", "O resultado tem de ser:" },
    { "That output is the whole specification. Any code that", "Esse resultado é toda a especificação. Qualquer código que" },
    { "That output is the whole specification. Any code", "Esse resultado é toda a especificação. Qualquer código" },
    { "That output is the whole specification. Any commands that", "Esse resultado é toda a especificação. Quaisquer comandos que" },
    { "That result is the whole specification. Anything that", "Esse resultado é toda a especificação. Tudo o que" },
    { "produces it is correct.", "o produza está certo." },
    { "that produces it is correct.", "que o produza está certo." },
    { "produce it are correct.", "o produzam estão certos." },
    { "produces it is correct. Open your file in whatever browser", "o produza está certo. Abre o teu ficheiro no navegador" },
    { "you normally use; this course never opens one.", "que costumas usar; este curso nunca abre nenhum." },
    { "Try it first. Compile with:", "Tenta primeiro. Compila com:" },
    { "Try it first. Run with:", "Tenta primeiro. Corre com:" },
    { "Try it first. Compile and run with:", "Tenta primeiro. Compila e corre com:" },
    { "Want to see example code?", "Queres ver código de exemplo?" },
    { "Want to see example commands?", "Queres ver comandos de exemplo?" },
    { "This is EXAMPLE CODE, not the answer. It is one way to get", "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de chegar" },
    { "This is EXAMPLE CODE, not the answer. It is one way to", "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de" },
    { "that output; yours may look nothing like it and still be", "a esse resultado; o teu pode ser muito diferente e estar" },
    { "that result; yours may look nothing like it and still be", "a esse resultado; o teu pode ser muito diferente e estar" },
    { "right -- or better. Compare the output, not the code.", "certo, ou melhor. Compara o resultado, não o código." },
    { "right -- or better.", "certo, ou melhor." },
    { "get that output; yours may look nothing like it and", "chegar a esse resultado; o teu pode ser muito diferente e" },
    { "still be right -- or better. Compare the output, not", "estar certo, ou melhor. Compara o resultado, não" },
    { "the code.", "o código." },
    { "This is EXAMPLE, not the answer. It is one way to reach that", "Isto é um EXEMPLO, não a resposta. É uma forma de chegar a esse" },
    { "result; a different sequence of commands landing on the same", "resultado; outra sequência de comandos que chegue ao mesmo" },
    { "state is correct too.", "estado também está certa." },
    { "Quit", "Sair" },
    { "Pick a module:", "Escolhe um módulo:" },
    { "Not a valid option.", "Opção inválida." },
    { "See you next time.", "Até à próxima." },
    { "Something went wrong:", "Algo correu mal:" },
    { "NO BROWSER -- this course is a CLI program, same as every", "SEM NAVEGADOR: este curso é um programa de terminal, como todos" },
    { "other course here. It teaches HTML; it never opens a page.", "os outros daqui. Ensina HTML; nunca abre uma página." },
    { "other course here. It teaches CSS; it never opens a page.", "os outros daqui. Ensina CSS; nunca abre uma página." },
    { "other course here. It teaches Web JavaScript; it never opens a page.", "os outros daqui. Ensina JavaScript web; nunca abre uma página." },
    { "NO GUI -- this course is a CLI program, same as every other", "SEM INTERFACE GRÁFICA: este curso é um programa de terminal," },
    { "course here. It teaches GTK; it does not open a window itself.", "como todos os outros. Ensina GTK; ele próprio não abre janelas." },
    { "SUMMARY", "RESUMO" },
    { "BASIC", "BÁSICO" },
    { "INTERMEDIATE", "INTERMÉDIO" },
    { "ADVANCED", "AVANÇADO" },
    { "CSS COURSE", "CURSO DE CSS" },
    { "The box and normal flow", "A caixa e o fluxo normal" },
    { "Flexbox", "Flexbox" },
    { "Grid", "Grid" },
    { "Responsive design", "Design responsivo" },
    { "Modern CSS and the cascade", "CSS moderno e a cascata" },
};

const char *tr(const char *text)
{
    if (lang_pt()) {
        for (size_t i = 0; i < sizeof TEXT / sizeof TEXT[0]; i++) {
            if (strcmp(TEXT[i][0], text) == 0) {
                return TEXT[i][1];
            }
        }
    }
    return text;
}

static const char *const SUMMARY_1[] = {
    "   - box-sizing: border-box faz width significar o que esperas.",
    "   - As margens verticais colapsam. As horizontais e os itens flex não.",
    "   - Os elementos inline ignoram width.",
    "   - position: absolute precisa de um antepassado posicionado, e",
    "     z-index precisa de position.",
    NULL,
};

static const char *const SUMMARY_2[] = {
    "   - justify-content segue o eixo principal, e flex-direction",
    "     decide qual é esse eixo.",
    "   - flex: 1 reparte o espaço que sobra.",
    "   - Os itens recusam encolher abaixo do conteúdo até definires",
    "     min-width: 0.",
    "   - flex-wrap: wrap deixa os itens passar para outra linha.",
    NULL,
};

static const char *const SUMMARY_3[] = {
    "   - fr é uma parte do espaço livre.",
    "   - repeat(auto-fit, minmax(...)) adapta-se sem media query.",
    "   - grid-template-areas desenha o layout em texto.",
    "   - minmax(0, 1fr) impede o conteúdo de alargar uma coluna.",
    NULL,
};

static const char *const SUMMARY_4[] = {
    "   - em multiplica quando aninhado. rem não.",
    "   - Primeiro as regras base, depois as media queries.",
    "   - clamp() dá limites a um valor dos dois lados.",
    "   - @container responde ao espaço disponível, não à janela.",
    NULL,
};

static const char *const SUMMARY_5[] = {
    "   - A especificidade compara-se coluna a coluna, e depois decide a",
    "     ordem no código.",
    "   - Um var() inválido cai para o valor herdado ou inicial, não para",
    "     a regra anterior.",
    "   - :is() assume o peso do seu argumento. :where() não pesa nada.",
    "   - Os estilos sem camada vencem os que estão em camadas.",
    "   - :has() seleciona pelo conteúdo, e as regras aninham-se com &.",
    NULL,
};

static const char *const *const SUMMARIES[] = {
    NULL,
    SUMMARY_1,
    SUMMARY_2,
    SUMMARY_3,
    SUMMARY_4,
    SUMMARY_5,
};

int print_summary_pt(int module)
{
    if (!lang_pt()) {
        return 0;
    }
    for (const char *const *line = SUMMARIES[module]; *line != NULL; line++) {
        puts(*line);
    }
    return 1;
}
