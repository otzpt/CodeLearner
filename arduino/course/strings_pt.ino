/*
 * Portuguese (PT-PT) text for the menus, prompts and module summaries.
 * CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.
 * It reads the PC's environment, so this tab only makes sense on the PC build.
 */

#include <cstdlib>
#include <cstring>

bool lang_pt()
{
    const char *value = getenv("CODELEARNER_LANG");
    return value != NULL && strcmp(value, "pt-pt") == 0;
}

const char *const TEXT[][2] = {
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
    { "ARDUINO COURSE - FROM setup() TO REGISTERS", "CURSO DE ARDUINO" },
    { "Your first sketch", "O teu primeiro sketch" },
    { "Variables and types", "Variáveis e tipos" },
    { "Decisions and loops", "Decisões e ciclos" },
    { "Functions and arrays", "Funções e arrays" },
    { "Digital pins", "Pinos digitais" },
    { "Reading Serial input", "Ler a entrada Serial" },
    { "Analog values and map()", "Valores analógicos e map()" },
    { "Time without freezing: millis()", "Tempo sem bloquear: millis()" },
    { "Text: char arrays and String", "Texto: arrays de char e String" },
    { "Memory, bits and ports", "Memória, bits e portas" },
    { "Interrupts and volatile", "Interrupções e volatile" },
    { "Inline assembly on the AVR", "Assembly inline no AVR" },
    { "Final project: an obstacle robot", "Projeto final: um robô que evita obstáculos" },
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

const char *const SUMMARY_1[] = {
    "   - um sketch tem setup() (uma vez) e loop() (para sempre), sem main()",
    "   - Serial.print fica na linha, Serial.println termina-a",
    "   - Serial.println(x, 4) define as casas decimais; (x, BIN) a base",
    "   - 'A' e 65 são coisas diferentes para o Serial",
    "",
    "  Módulo 2: o tamanho de um número num chip tão pequeno.",
    NULL,
};

const char *const SUMMARY_2[] = {
    "   - um int tem 2 bytes num Uno, 4 neste PC: indica o tamanho",
    "     com uint8_t, int16_t, uint32_t quando importa",
    "   - os unsigned dão a volta a 0; overflow em signed é indefinido",
    "   - int / int perde a parte decimal; converte um dos lados PRIMEIRO",
    "   - um float guarda cerca de 7 dígitos",
    "",
    "  Módulo 3: tomar decisões e repetir.",
    NULL,
};

const char *const SUMMARY_3[] = {
    "   - if / else decide; == compara, = atribui",
    "   - for(início; continua?; passo) e while(continua?)",
    "   - i < 4 dá quatro passagens, de 0 a 3",
    "   - && || !  e  break",
    "",
    "  Módulo 4: funções e arrays.",
    NULL,
};

const char *const SUMMARY_4[] = {
    "   - uma função recebe cópias dos argumentos",
    "   - os arrays contam a partir de 0; nada impede um índice depois do fim",
    "   - sizeof(a) / sizeof(a[0]) conta elementos, mas só onde o",
    "     array é declarado",
    "   - passa a contagem juntamente com o array",
    "",
    "  Módulo 5: a primeira ideia real de hardware: pinos.",
    NULL,
};

const char *const SUMMARY_5[] = {
    "   - pinMode primeiro (OUTPUT, INPUT ou INPUT_PULLUP), depois usa-o",
    "   - HIGH é 1 e LOW é 0; LED_BUILTIN é o pino 13 num Uno",
    "   - INPUT_PULLUP inverte um botão: premido lê LOW",
    "   - digitalWrite num pino INPUT define o seu pull-up",
    "   - delay() congela tudo o resto",
    "",
    "  Módulo 6: responder: ler a entrada Serial.",
    NULL,
};

const char *const SUMMARY_6[] = {
    "   - Serial.available() e depois Serial.read(): um carácter, -1",
    "     quando não há nenhum",
    "   - ENTER é um carácter (10); um monitor pode acrescentar 13",
    "   - parseInt() devolve 0 para 'sem número', igual a um 0 real",
    "   - trata sempre o comando que não esperavas",
    "",
    "  Módulo 7: valores analógicos, e passar de um intervalo a outro.",
    NULL,
};

const char *const SUMMARY_7[] = {
    "   - analogRead é 0-1023; analogWrite é 0-255 e só em pinos PWM",
    "   - PWM é ligar e desligar depressa, não uma tensão real",
    "   - num chip de int de 16 bits, alarga para 32 bits ANTES de multiplicar",
    "   - map() trunca e não limita; usa constrain() a seguir",
    "",
    "  Módulo 8: esperar sem bloquear a placa.",
    NULL,
};

const char *const SUMMARY_8[] = {
    "   - delay() deixa o chip surdo; millis() deixa-o continuar a trabalhar",
    "   - if (millis() - last >= interval) { last += interval; ... }",
    "   - millis() dá a volta ao fim de cerca de 49,7 dias",
    "   - subtrai sempre: millis() - start, nunca compares com uma soma",
    "",
    "  Módulo 9: texto, e o que custa num chip tão pequeno.",
    NULL,
};

const char *const SUMMARY_9[] = {
    "   - texto é um array de char com um zero no fim: N letras, N+1",
    "   - strlen conta letras, sizeof conta o array",
    "   - String é confortável mas vive na heap: evita-a em código que",
    "     corre durante dias",
    "   - indexOf, substring e toInt, ou sscanf, retiram números",
    "",
    "  Módulo 10: memória, bits e os registos do próprio chip.",
    NULL,
};

const char *const SUMMARY_10[] = {
    "   - a flash guarda o programa, a SRAM as variáveis (só 2 KB),",
    "     a EEPROM algumas definições guardadas",
    "   - F() e PROGMEM mantêm texto e tabelas fora da SRAM",
    "   - |= liga, &= ~ desliga, ^= inverte, (x >> n) & 1 testa um bit;",
    "     põe parênteses nos testes com & e |",
    "   - DDRx, PORTx, PINx são os pinos; usa |= e &= neles",
    "",
    "  Módulo 11: código que corre no instante em que algo acontece.",
    NULL,
};

const char *const SUMMARY_11[] = {
    "   - uma ISR corre logo num evento de pino; mantém-na minúscula",
    "   - variáveis partilhadas com uma ISR têm de ser volatile",
    "   - copia variáveis partilhadas de vários bytes com as interrupções desligadas",
    "   - eventos durante noInterrupts() juntam-se num só",
    "",
    "  Módulo 12: escrever instruções à mão dentro do C++.",
    NULL,
};

const char *const SUMMARY_12[] = {
    "   - vê a listagem do compilador antes de escrever assembly",
    "   - asm volatile(\"nop\") é um ciclo: 62,5 ns a 16 MHz",
    "   - asm estendido: template, outputs, inputs, clobbers; r, d, I, M",
    "   - guarda e repõe SREG à volta de uma secção crítica",
    "   - um ficheiro .S na pasta do sketch é uma função inteira em assembly",
    "",
    "  Módulo 13: tudo junto, num robô.",
    NULL,
};

const char *const SUMMARY_13[] = {
    "   - divide o problema: ler o sensor, decidir, conduzir",
    "   - uma variável de modo mais millis() é uma máquina de estados",
    "     que nunca bloqueia",
    "   - trata 'sem eco' como longe, não como zero",
    "   - funções pequenas e puras como choose_speed são fáceis de testar",
    "",
    "  É o curso. O curso de MicroPython cobre as mesmas ideias em",
    "  Python para o Raspberry Pi Pico.",
    NULL,
};

const char *const *const SUMMARIES[] = {
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

bool print_summary_pt(int module)
{
    if (!lang_pt()) {
        return false;
    }
    for (const char *const *line = SUMMARIES[module]; *line != NULL; line++) {
        Serial.println(*line);
    }
    return true;
}
