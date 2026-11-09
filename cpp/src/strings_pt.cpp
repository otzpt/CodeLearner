/*
 * Portuguese (PT-PT) text for the menus, prompts and module summaries.
 * CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
    { "C++ COURSE - FROM C TO CLASSES", "CURSO DE C++" },
    { "Compiling and printing", "Compilar e escrever" },
    { "Variables and types", "Variáveis e tipos" },
    { "Reading input", "Ler entrada" },
    { "Conditions", "Condições" },
    { "Loops", "Ciclos" },
    { "std::string", "std::string" },
    { "Functions and references", "Funções e referências" },
    { "std::vector", "std::vector" },
    { "Classes and RAII", "Classes e RAII" },
    { "Maps and sets", "Maps e sets" },
    { "Templates", "Templates" },
    { "Modern C++ idioms", "Idiomas do C++ moderno" },
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
    "   - g++, não gcc; .cpp, não .c",
    "   - std::cout << valor, encadeado, sem string de formato",
    "   - \\n por omissão, std::endl quando precisas de despejar já",
    "",
    "  Módulo 2: variáveis, e uma armadilha que não desapareceu.",
    NULL,
};

static const char *const SUMMARY_2[] = {
    "   - std::string e auto são as duas melhorias reais aqui",
    "   - int / int continua a truncar: converte primeiro um dos lados",
    "   - static_cast<T>(x) é a forma moderna de escrever (T)x",
    NULL,
};

static const char *const SUMMARY_3[] = {
    "   - cin >> x não precisa de &; getline(cin, str) não tem limite de tamanho",
    "   - misturar >> e getline deixa um fim de linha para trás: usa ignore()",
    "   - !(cin >> x) é verdadeiro quando a leitura falhou",
    NULL,
};

static const char *const SUMMARY_4[] = {
    "   - corre o primeiro ramo verdadeiro; a ordem importa, como antes",
    "   - bool é um tipo real; cout continua a mostrá-lo como 1/0 sem",
    "     boolalpha",
    "   - = continua a não ser ==, e o compilador continua só a avisar",
    NULL,
};

static const char *const SUMMARY_5[] = {
    "   - for/while mantêm-se iguais, incluindo o erro de um a mais",
    "   - o for baseado em intervalos elimina o índice",
    "   - `for (auto x : ...)` copia; `for (auto &x : ...)` não",
    "",
    "  Módulo 6: std::string, e tudo o que o strcmp tornava penoso.",
    NULL,
};

static const char *const SUMMARY_6[] = {
    "   - == compara conteúdo; + concatena; não há \\0 a gerir",
    "   - find devolve std::string::npos, não -1, quando não encontra",
    "   - [] não verifica, como em C; at() verifica e lança exceção",
    NULL,
};

static const char *const SUMMARY_7[] = {
    "   - parâmetros simples continuam a ser cópias",
    "   - & torna um parâmetro um alias, sem & ao chamar",
    "   - const & passa tipos grandes sem copiar e sem risco",
    "   - a sobrecarga escolhe a função pelos tipos dos argumentos",
    NULL,
};

static const char *const SUMMARY_8[] = {
    "   - push_back faz crescer o vector; size() está sempre certo",
    "   - std::sort(v.begin(), v.end()) ordena tudo",
    "   - [] não verifica, at() lança exceção: a regra de std::string",
    "",
    "  O módulo 9 fecha o círculo que o módulo 9 do C abriu: memória",
    "  que se liberta sozinha, e corretamente.",
    NULL,
};

static const char *const SUMMARY_9[] = {
    "   - class acrescenta métodos e private/public à struct",
    "   - o construtor corre na criação, o destrutor no fim do âmbito",
    "   - RAII: recursos ligados à vida do objeto, libertados automaticamente",
    "   - new/delete continuam a ser malloc/free, e vazam se não baterem certo",
    "",
    "  Fica assim o curso principal. std::string, std::vector e as",
    "  tuas classes cobrem a grande maioria do que em C exigia",
    "  gestão manual de memória.",
    "",
    "  O módulo 10 completa a STL com os dois contentores que o",
    "  vector não substitui: std::map e std::set.",
    NULL,
};

static const char *const SUMMARY_10[] = {
    "   - std::map<K, V>: chave para valor, ordenado por chave",
    "   - o [] de map insere um valor por omissão numa chave em falta:",
    "     usa .count() para verificar sem inserir",
    "   - std::set<T>: cada valor no máximo uma vez, ordenado",
    "   - std::find devolve end() num contentor quando não",
    "     encontra o valor",
    "",
    "  O módulo 11 são templates: o mecanismo que deixa existir",
    "  std::vector<int> e std::vector<std::string> a partir do",
    "  mesmo código, sem escrever um por tipo.",
    NULL,
};

static const char *const SUMMARY_11[] = {
    "   - um template é uma definição, compilada à parte para cada tipo usado",
    "   - template<typename T> serve para funções e para classes",
    "   - a dedução de argumentos lê T dos argumentos, sem precisar de <T>",
    "   - um tipo sem uma operação de que o template precisa falha ao compilar",
    "",
    "  std::vector<T>, std::map<K, V> e std::string já eram todos",
    "  templates desde o módulo 5: este é o mecanismo que esteve",
    "  sempre a funcionar por baixo.",
    "",
    "  O módulo 12 é o último: as idiomáticas que dão ao C++ moderno",
    "  o aspeto que tem, a começar pela fuga de new/delete do",
    "  módulo 9, resolvida de vez.",
    NULL,
};

static const char *const SUMMARY_12[] = {
    "   - std::unique_ptr<T>: dono único, destruído automaticamente,",
    "     copiá-lo não compila",
    "   - std::shared_ptr<T>: posse partilhada, contada com use_count(),",
    "     custa trabalho real por cópia: prefere unique_ptr",
    "   - std::move converte para referência rvalue; não move nada",
    "     sozinho, só permite que o construtor de movimento corra",
    "   - um objeto movido fica válido mas indefinido, não destruído",
    "     nem proibido de tocar",
    "   - auto [a, b] = pair; separa um par ou entrada de map em nomes,",
    "     em vez de .first e .second",
    "",
    "  É o curso todo: de um compilador que só conhecia printf, a",
    "  memória que se gere sozinha e posse que o sistema de tipos",
    "  consegue impor.",
    NULL,
};

static const char *const SUMMARY_13[] = {
    "   - o asm estendido é igual ao do C; procura primeiro std:: ou um builtin",
    "   - uma instrução asm não pode ser avaliada numa função constexpr",
    "   - funções NASM precisam de extern \"C\" ou o linker procura um nome decorado",
    "   - o ABI System V: argumentos em rdi, rsi, rdx, rcx, r8, r9; resultado em rax",
    "",
    "  É o curso todo, até à instrução.",
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
