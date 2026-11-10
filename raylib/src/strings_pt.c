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
    { "course here. It teaches raylib and raygui; it does not open a window itself.", "como todos os outros. Ensina raylib e raygui; ele próprio não abre janelas." },
    { "SUMMARY", "RESUMO" },
    { "BASIC", "BÁSICO" },
    { "INTERMEDIATE", "INTERMÉDIO" },
    { "ADVANCED", "AVANÇADO" },
    { "RAYLIB COURSE", "CURSO DE RAYLIB" },
    { "raylib: your first window and the loop", "raylib: a tua primeira janela e o ciclo" },
    { "raylib: shapes, colours and text", "raylib: formas, cores e texto" },
    { "raylib: keyboard, mouse and clicks", "raylib: teclado, rato e cliques" },
    { "raylib: time, movement and collision", "raylib: tempo, movimento e colisão" },
    { "raygui: buttons, text and state", "raygui: botões, texto e estado" },
    { "raygui: a todo list", "raygui: uma lista de tarefas" },
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
    "   - o raylib redesenha o frame inteiro num ciclo; nada é guardado",
    "     por ti entre frames",
    "   - SetTargetFPS limita o ciclo: 60 frames por segundo contra",
    "     12134 medidos sem ele",
    "   - desenha entre BeginDrawing e EndDrawing; o y cresce para baixo",
    "   - um ficheiro em falta é um WARNING registado, não um crash",
    NULL,
};

static const char *const SUMMARY_2[] = {
    "   - a origem é o canto superior esquerdo e o y cresce para baixo;",
    "     as formas desenhadas depois cobrem as anteriores",
    "   - um retângulo cobre de x até x + largura - 1",
    "   - usa Fade() para transparência: um alfa fracionário vira 0",
    "   - os triângulos precisam de pontos no sentido anti-horário, ou",
    "     desaparecem",
    "   - centra o texto com MeasureText",
    NULL,
};

static const char *const SUMMARY_3[] = {
    "   - pergunta a cada frame: Down é verdadeiro em todas as frames em",
    "     que está premido, Pressed numa só, Released numa só",
    "   - um clique é Pressed mais um teste de colisão com o ponteiro",
    "   - uma caixa inclui as arestas de cima e da esquerda e exclui",
    "     x + largura e y + altura",
    "   - GetCharPressed para texto escrito, constantes de tecla para",
    "     controlos",
    NULL,
};

static const char *const SUMMARY_4[] = {
    "   - move por velocidade * GetFrameTime(), nunca por uma quantidade",
    "     fixa por frame (93 px contra 183 px medidos)",
    "   - normaliza uma direção antes de a multiplicar pela velocidade",
    "   - arestas que só se tocam não colidem; sobrepor colide",
    "   - mantém as regras numa função sem desenho",
    NULL,
};

static const char *const SUMMARY_5[] = {
    "   - um widget do raygui é uma chamada de função que desenha e",
    "     responde de uma vez; as tuas variáveis guardam o estado",
    "   - GuiCheckBox recebe um ponteiro; GuiTextBox recebe o tamanho",
    "     real do buffer e és tu que alternas o editMode",
    "   - o resultado do TextFormat é um buffer reutilizado: usa-o logo",
    "   - GuiSetStyle recebe ints; ColorToInt converte uma Color",
    NULL,
};

static const char *const SUMMARY_6[] = {
    "   - a lista é um array; a janela redesenha-o a cada frame",
    "   - mantém as funções de dados sem desenho, para poderem ser",
    "     testadas",
    "   - nunca apagues dentro do ciclo que desenha: regista o pedido e",
    "     apaga depois",
    "   - o strncpy pode não terminar a string; termina-a tu",
    "",
    "  Não coberto: scroll de listas longas, outros controlos do raygui",
    "  (sliders, listas) e carregar imagens e som com o raylib. A folha",
    "  de consulta do raylib lista-os.",
    NULL,
};

static const char *const *const SUMMARIES[] = {
    NULL,
    SUMMARY_1,
    SUMMARY_2,
    SUMMARY_3,
    SUMMARY_4,
    SUMMARY_5,
    SUMMARY_6,
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
