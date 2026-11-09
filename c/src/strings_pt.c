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
    { "C COURSE - FROM ZERO TO MEMORY", "CURSO DE C" },
    { "Compiling and printing", "Compilar e escrever" },
    { "Variables and types", "Variáveis e tipos" },
    { "Reading input", "Ler entrada" },
    { "Conditions", "Condições" },
    { "Loops", "Ciclos" },
    { "Arrays and strings", "Arrays e strings" },
    { "Functions", "Funções" },
    { "Pointers", "Apontadores" },
    { "Memory: malloc and free", "Memória: malloc e free" },
    { "Structs", "Structs" },
    { "Final test: 2 programs", "Teste final: 2 programas" },
    { "Growing memory: grids and realloc", "Memória que cresce: grelhas e realloc" },
    { "Linked lists", "Listas ligadas" },
    { "Stacks and queues", "Pilhas e filas" },
    { "Enums, unions, and typedef", "Enums, unions e typedef" },
    { "Compilation and project structure", "Compilação e estrutura de projeto" },
    { "Debugging", "Depuração" },
    { "Real input/output", "Entrada e saída reais" },
    { "Function pointers, callbacks, and macros", "Apontadores para funções, callbacks e macros" },
    { "Inline assembly and NASM", "Assembly inline e NASM" },
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
    "   - C compila antes de correr",
    "   - todos os programas começam em main",
    "   - printf escreve, \\n muda de linha",
    "   - -Wall -Wextra, sempre",
    "",
    "  Módulo 2: guardar valores em variáveis.",
    NULL,
};

static const char *const SUMMARY_2[] = {
    "   - o tipo vem antes do nome e nunca muda",
    "   - cada tipo tem o seu marcador no printf",
    "   - int / int dá int, mesmo guardado num float",
    "   - converte antes de dividir, não depois",
    NULL,
};

static const char *const SUMMARY_3[] = {
    "   - scanf leva & nos números, sem & nos arrays",
    "   - sem & -> Segmentation fault",
    "   - limita sempre o tamanho: %49s para um array de 50",
    "   - fgets é mais seguro e lê a linha inteira",
    NULL,
};

static const char *const SUMMARY_4[] = {
    "   - só corre o primeiro ramo verdadeiro; a ordem importa",
    "   - == compara, = atribui",
    "   - 0 é falso, tudo o resto é verdadeiro",
    "   - chavetas, sempre",
    "   - switch precisa de break em cada case, ou cai no seguinte",
    NULL,
};

static const char *const SUMMARY_5[] = {
    "   - for: início; condição; passo",
    "   - os índices vão de 0 a n-1: usa <, não <=",
    "   - C não verifica os limites dos arrays",
    "   - algo dentro de um while tem de mudar a condição",
    "   - do-while verifica depois do corpo, por isso corre sempre pelo menos uma vez",
    "",
    "  O módulo 6 começa a parte que distingue o C: arrays,",
    "  memória e apontadores.",
    NULL,
};

static const char *const SUMMARY_6[] = {
    "   - array = valores lado a lado na memória",
    "   - string = array de char terminado em \\0",
    "   - strlen conta letras, sizeof conta bytes",
    "   - == compara endereços; strcmp(a,b)==0 compara texto",
    NULL,
};

static const char *const SUMMARY_7[] = {
    "   - tipo nome(parâmetros) { ... return ...; }",
    "   - os argumentos são cópias",
    "   - passa &variavel para a função poder mudar a tua",
    "   - declara antes de usar, ou escreve um protótipo",
    "   - uma variável local esconde uma global com o mesmo nome, não a altera",
    "   - a recursão precisa de um caso base alcançável, ou a pilha esgota-se",
    NULL,
};

static const char *const SUMMARY_8[] = {
    "   - um apontador é uma variável que guarda um endereço",
    "   - & obtém o endereço, * lê ou escreve o que lá está",
    "   - num tipo, * significa 'apontador'; no código, * acede ao valor",
    "   - testa NULL antes de usar",
    "   - const int *p impede escrever através de p; int *const p fixa o próprio p",
    "",
    "  O módulo 9 usa isto para pedir memória como deve ser.",
    NULL,
};

static const char *const SUMMARY_9[] = {
    "   - pilha: automática, morre com a função",
    "   - heap: malloc, vive até ao free",
    "   - verifica sempre se o malloc devolveu NULL",
    "   - um free por cada malloc, depois p = NULL",
    "   - nunca devolvas o endereço de um array local",
    "   - gcc -fsanitize=address diz-te onde erraste",
    NULL,
};

static const char *const SUMMARY_10[] = {
    "   - struct agrupa campos num só tipo; ; depois da chaveta",
    "   - a.campo com uma struct, ptr->campo com um apontador",
    "   - -> é (*ptr). escrito de outra forma",
    "   - sizeof pode ser maior do que a soma dos campos (padding)",
    "",
    "  Fica assim o curso base. O passo seguinte é escrever um programa",
    "  teu que use malloc e o liberte.",
    NULL,
};

static const char *const SUMMARY_11[] = {
    "   - o teste final só acrescenta rand()/srand(): o resto são",
    "     os módulos 1 a 10, combinados em dois programas completos",
    "   - faz malloc de uma quantidade decidida em execução, e liberta-a antes de sair",
    "   - passa structs por apontador a uma função quando tiverem",
    "     mais do que um par de campos",
    "",
    "  Os módulos 12 e 13 vão mais longe na memória, se quiseres:",
    "  apontadores para apontadores e realloc num, listas ligadas",
    "  no outro.",
    NULL,
};

static const char *const SUMMARY_12[] = {
    "   - int **p é um apontador para um apontador; liberta de dentro para fora,",
    "     cada linha antes do array de linhas",
    "   - faz sempre realloc para uma variável temporária, nunca",
    "     para a mesma que estás a redimensionar",
    "   - duplicar a capacidade quando enche mantém poucos reallocs,",
    "     mesmo num array que cresce muito",
    "",
    "  Agora tens tudo o que o módulo 9 prometeu: sabes de onde veio",
    "  cada byte que pediste, e para onde foi.",
    "",
    "  O módulo 13 é uma segunda forma de fazer crescer uma coleção:",
    "  uma lista ligada, um nó de cada vez, em vez dos blocos",
    "  duplicados do realloc.",
    NULL,
};

static const char *const SUMMARY_13[] = {
    "   - uma struct pode apontar para o seu próprio tipo; é isso",
    "     uma lista ligada",
    "   - inserir à cabeça é O(1), mas inverte a ordem de chegada",
    "   - guarda ->next antes de libertar um nó, ou perdes o acesso",
    "     ao resto da lista",
    "",
    "  O módulo 14 parte daqui: duas estruturas de dados, pilhas e",
    "  filas, com um array simples em vez de outra lista ligada.",
    NULL,
};

static const char *const SUMMARY_14[] = {
    "   - uma pilha é LIFO: push e pop acontecem ambos em `top`",
    "   - uma fila é FIFO: enqueue atrás, dequeue à frente",
    "   - uma pilha ou fila com array precisa de verificar a capacidade",
    "     antes de escrever: \"stack overflow\" é essa verificação",
    "     em falta, não é uma metáfora",
    "   - uma fila ingénua com array diz-se cheia muito antes de o",
    "     estar, porque `front` e `rear` só andam para a frente",
    "   - % (módulo) faz o índice voltar a 0, e é isso que torna",
    "     uma fila ingénua numa fila correta e reutilizável",
    "",
    "  Ambas usam um array de tamanho fixo de propósito. Uma lista",
    "  ligada (como a do módulo 13) elimina o limite de capacidade,",
    "  à custa de um malloc por elemento em vez de um para o bloco",
    "  todo: o mesmo compromisso que o módulo 9 levantou primeiro",
    "  entre um array e uma estrutura ligada.",
    NULL,
};

static const char *const SUMMARY_15[] = {
    "   - enum dá nome a um conjunto de constantes int e dá ao próprio",
    "     enum um nome de tipo real; ao contrário de #define, uma",
    "     variável desse tipo continua visível num depurador (C não",
    "     verifica se só lá entra um membro válido)",
    "   - os membros de uma union partilham o mesmo espaço; sizeof é o",
    "     do maior membro, não a soma, e só o último escrito é",
    "     seguro de ler",
    "   - typedef dá nome a um tipo existente; compensa em",
    "     struct/enum/union, porque evita a palavra-chave em cada uso",
    "   - o padding é uma propriedade do compilador e da máquina,",
    "     não da linguagem: vale para um int (módulo 10) e",
    "     igualmente para um enum",
    "",
    "  Isto cobre todos os tipos que este curso constrói à mão.",
    "  O que falta é prática: programas maiores que combinem tudo,",
    "  como o teste final do módulo 11 combinou os módulos 1 a 10.",
    NULL,
};

static const char *const SUMMARY_16[] = {
    "   - um .h é uma promessa (declarações); um .c é o seu cumprimento",
    "     (definições); o mesmo .h pode ser incluído por vários .c",
    "   - uma guarda de cabeçalho (#ifndef/#define/#endif) impede que",
    "     o conteúdo de um cabeçalho seja definido duas vezes quando",
    "     é incluído mais do que uma vez",
    "   - cc -c compila um ficheiro para um .o por ligar; o linker",
    "     junta os .o num executável, e é dele que vem",
    "     \"undefined reference\", não do compilador",
    "   - -Wall -Wextra apanham bugs reais sem nunca recusar compilar:",
    "     um aviso é um conselho, não um erro",
    "   - -g acrescenta símbolos para um depurador (módulo 17); nunca",
    "     muda o que o programa calcula",
    "   - um Makefile automatiza a mesma sequência de compilar e ligar",
    "     que a PARTE 3 fez à mão; uma biblioteca estática junta código",
    "     já compilado para o linker o trazer pelo nome",
    "",
    "  O módulo 17 continua onde as PARTES 4 e 5 ficaram: ler estes",
    "  mesmos diagnósticos num depurador e num sanitizer a sério,",
    "  em vez de só no terminal.",
    NULL,
};

static const char *const SUMMARY_17[] = {
    "   - um compilador só lê texto; gdb e um sanitizer veem o programa",
    "     a correr, por isso reportam o que só aparece em execução",
    "   - gdb pausa um programa com um breakpoint, avança linha a",
    "     linha com step/next, e lê valores reais com print e",
    "     info locals, sem nunca mudar o que ele calcula",
    "   - um backtrace lista cada frame ainda na pilha de chamadas,",
    "     #0 é o mais interior: a função que estava a correr,",
    "     depois quem a chamou, até ao main()",
    "   - escrever num apontador NULL falha sempre e é seguro correr",
    "     a sério; um use-after-free não é, e por isso este módulo",
    "     o compilou com AddressSanitizer desde o início",
    "   - AddressSanitizer só verifica código que corre, e só certas",
    "     classes de bug; -fsanitize=undefined cobre outra família:",
    "     juntos, uma opção cada",
    "   - a mesma receita serve para o que escreveres a seguir:",
    "     cc -fsanitize=address,undefined yourfile.c -o test && ./test",
    NULL,
};

static const char *const SUMMARY_18[] = {
    "   - fopen devolve NULL em caso de falha; verifica antes de usar o",
    "     FILE *, a mesma disciplina que o módulo 9 ensinou para malloc",
    "   - fprintf escreve num ficheiro como printf escreve no ecrã;",
    "     fgets lê um ficheiro como lê o teclado: ambos devolvem",
    "     NULL ou param no fim da entrada",
    "   - \"r\" e \"rb\" abrem de forma idêntica em Linux, verificado acima:",
    "     a diferença é real e só aparece na tradução \\r\\n do Windows",
    "   - verificar o valor de retorno do scanf apanha entrada inválida",
    "     que o scanf já viu; um ciclo a sério que volta a perguntar",
    "     também tem de limpar o resto do texto, ou repete para sempre",
    "   - int main(int argc, char *argv[]) é a razão de haver duas",
    "     assinaturas de main válidas; argc conta argv[0], o nome do",
    "     próprio programa, e argv[argc] é sempre NULL",
    NULL,
};

static const char *const SUMMARY_19[] = {
    "   - o nome de uma função decai para o seu endereço; um apontador",
    "     para função pode guardá-lo e chamar através dele, tal como a",
    "     tabela MODULES deste curso chama cada lição",
    "   - um callback é um parâmetro apontador para função: o código",
    "     que o chama nunca precisa de saber o que ele faz",
    "   - uma função variádica não sabe por si quantos argumentos tem;",
    "     tem de ser algo controlado por quem chama a dizê-lo (uma",
    "     contagem explícita, ou a string de formato do printf)",
    "   - uma macro com parâmetros substitui tokens, não valores",
    "     avaliados: ((x)*(x)) é o que impede que os operadores de",
    "     quem chama entrem sem parênteses",
    "   - &, |, ^, ~, <<, >> trabalham nos bits reais de um int: & testa",
    "     um, | liga-o, ^ inverte-o, e & com uma máscara negada",
    "     desliga-o sem tocar em mais nenhum bit",
    NULL,
};

static const char *const SUMMARY_20[] = {
    "   - __asm__(template : outputs : inputs : clobbers), AT&T por omissão",
    "   - r / m / i dizem o que um operando pode ser; = escreve, + lê e escreve",
    "   - um output escrito cedo precisa de &; alterações por declarar precisam de clobbers",
    "   - ficheiros NASM usam sintaxe Intel e ligam-se através do ABI System V",
    "",
    "  O passo seguinte seria ler o que o próprio compilador escreve:",
    "  gcc -S -O2 file.c, e comparar com o que terias escrito.",
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
    SUMMARY_7,
    SUMMARY_8,
    SUMMARY_9,
    SUMMARY_10,
    SUMMARY_11,
    SUMMARY_12,
    SUMMARY_13,
    SUMMARY_14,
    SUMMARY_15,
    SUMMARY_16,
    SUMMARY_17,
    SUMMARY_18,
    SUMMARY_19,
    SUMMARY_20,
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
