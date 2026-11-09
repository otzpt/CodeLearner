"""Portuguese (PT-PT) text for the menus, prompts and module summaries.

CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.
"""

import os


def _pt_mode():
    # MicroPython's os has getenv on the unix port, and no environ.
    return os.getenv("CODELEARNER_LANG") == "pt-pt"


PT = _pt_mode()

TEXT = {
    "Press ENTER to continue...": "Prime ENTER para continuar...",
    "Your answer:": "A tua resposta:",
    "CORRECT.": "CERTO.",
    "NOT QUITE. The answer is:": "NÃO É BEM ISSO. A resposta é:",
    ">> EXERCISE - MODULE": ">> EXERCÍCIO - MÓDULO",
    ">> WRITE THIS YOURSELF, in a real file": ">> ESCREVE ISTO TU, num ficheiro a sério",
    ">> TRY THIS YOURSELF, in a real directory": ">> EXPERIMENTA TU, numa pasta a sério",
    "Try it with this input:": "Experimenta com esta entrada:",
    "It must print:": "Tem de escrever:",
    "You should see:": "Deves ver:",
    "The result must be:": "O resultado tem de ser:",
    "That output is the whole specification. Any code that": "Esse resultado é toda a especificação. Qualquer código que",
    "That output is the whole specification. Any code": "Esse resultado é toda a especificação. Qualquer código",
    "That output is the whole specification. Any commands that": "Esse resultado é toda a especificação. Quaisquer comandos que",
    "That result is the whole specification. Anything that": "Esse resultado é toda a especificação. Tudo o que",
    "produces it is correct.": "o produza está certo.",
    "that produces it is correct.": "que o produza está certo.",
    "produce it are correct.": "o produzam estão certos.",
    "produces it is correct. Open your file in whatever browser": "o produza está certo. Abre o teu ficheiro no navegador",
    "you normally use; this course never opens one.": "que costumas usar; este curso nunca abre nenhum.",
    "Try it first. Compile with:": "Tenta primeiro. Compila com:",
    "Try it first. Run with:": "Tenta primeiro. Corre com:",
    "Try it first. Compile and run with:": "Tenta primeiro. Compila e corre com:",
    "Want to see example code?": "Queres ver código de exemplo?",
    "Want to see example commands?": "Queres ver comandos de exemplo?",
    "This is EXAMPLE CODE, not the answer. It is one way to get": "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de chegar",
    "This is EXAMPLE CODE, not the answer. It is one way to": "Isto é CÓDIGO DE EXEMPLO, não a resposta. É uma forma de",
    "that output; yours may look nothing like it and still be": "a esse resultado; o teu pode ser muito diferente e estar",
    "that result; yours may look nothing like it and still be": "a esse resultado; o teu pode ser muito diferente e estar",
    "right -- or better. Compare the output, not the code.": "certo, ou melhor. Compara o resultado, não o código.",
    "right -- or better.": "certo, ou melhor.",
    "get that output; yours may look nothing like it and": "chegar a esse resultado; o teu pode ser muito diferente e",
    "still be right -- or better. Compare the output, not": "estar certo, ou melhor. Compara o resultado, não",
    "the code.": "o código.",
    "This is EXAMPLE, not the answer. It is one way to reach that": "Isto é um EXEMPLO, não a resposta. É uma forma de chegar a esse",
    "result; a different sequence of commands landing on the same": "resultado; outra sequência de comandos que chegue ao mesmo",
    "state is correct too.": "estado também está certa.",
    "Quit": "Sair",
    "Pick a module:": "Escolhe um módulo:",
    "Not a valid option.": "Opção inválida.",
    "See you next time.": "Até à próxima.",
    "Something went wrong:": "Algo correu mal:",
    "NO BROWSER -- this course is a CLI program, same as every": "SEM NAVEGADOR: este curso é um programa de terminal, como todos",
    "other course here. It teaches HTML; it never opens a page.": "os outros daqui. Ensina HTML; nunca abre uma página.",
    "other course here. It teaches CSS; it never opens a page.": "os outros daqui. Ensina CSS; nunca abre uma página.",
    "other course here. It teaches Web JavaScript; it never opens a page.": "os outros daqui. Ensina JavaScript web; nunca abre uma página.",
    "NO GUI -- this course is a CLI program, same as every other": "SEM INTERFACE GRÁFICA: este curso é um programa de terminal,",
    "course here. It teaches GTK; it does not open a window itself.": "como todos os outros. Ensina GTK; ele próprio não abre janelas.",
    "SUMMARY": "RESUMO",
    "BASIC": "BÁSICO",
    "INTERMEDIATE": "INTERMÉDIO",
    "ADVANCED": "AVANÇADO",
    "MICROPYTHON COURSE - FROM print() TO THE PICO": "CURSO DE MICROPYTHON",
    "Running MicroPython and print()": "Correr MicroPython e print()",
    "Numbers, text and variables": "Números, texto e variáveis",
    "Decisions, loops and functions": "Decisões, ciclos e funções",
    "Lists, bytes and memory": "Listas, bytes e memória",
    "Pins: machine.Pin": "Pinos: machine.Pin",
    "ADC and PWM": "ADC e PWM",
    "Time without freezing": "Tempo sem bloquear",
    "Interrupts": "Interrupções",
    "Speed: native and viper": "Velocidade: native e viper",
    "Inline assembly on the Pico": "Assembly inline no Pico",
    "Final project: an obstacle robot": "Projeto final: um robô que evita obstáculos",
}

SUMMARIES = {
    1: [
        "   - MicroPython corre o teu .py diretamente no chip: boot.py, depois main.py",
        "   - a mesma linguagem que Python 3, com biblioteca padrão mais pequena",
        "   - um método em falta só falha em execução, quando a linha é alcançada",
        "   - o REPL testa uma linha de cada vez; Ctrl-C para um programa",
        "",
        "  Módulo 2: números, e porque um float não é igual no chip.",
    ],
    2: [
        "   - os inteiros nunca dão overflow; / dá sempre float, // arredonda para baixo",
        "   - um float tem 64 bits neste PC e normalmente 32 numa placa",
        "   - 1.0 + 1e-8 - 1.0 distingue os dois",
        "   - str são caracteres, bytes são dados na ligação; encode() e decode()",
        "",
        "  Módulo 3: decisões, ciclos e funções.",
    ],
    3: [
        "   - a indentação é o bloco; 0, '', [] e None são falsos",
        "   - range(a, b) para antes de b; while True é o ciclo principal do robô",
        "   - atribuir a uma global dentro de uma função torna-a local",
        "",
        "  Módulo 4: listas, bytes, e a memória que usam.",
    ],
    4: [
        "   - b = a partilha uma lista; a[:] copia-a",
        "   - bytearray: um byte por elemento, alterável no sítio",
        "   - guardar um valor fora do intervalo num bytearray é mascarado em silêncio",
        "   - memoryview corta sem copiar; gc.collect() antes de medir tempos",
        "",
        "  Módulo 5: pinos, o primeiro hardware a sério.",
    ],
    5: [
        "   - Pin(n, Pin.OUT) controla; Pin(n, Pin.IN, Pin.PULL_UP) lê",
        "   - a lógica do Pico é de 3.3 V; GP25 é o LED num Pico, Pin('LED') num Pico W",
        "   - um botão com pull-up lê 0 quando premido",
        "   - os botões reais têm ressalto: ignora flancos que venham cedo demais",
        "",
        "  Módulo 6: valores analógicos, e PWM.",
    ],
    6: [
        "   - ADC(Pin(26)).read_u16() é 0-65535 para 0-3.3 V",
        "   - PWM(Pin(n)): freq() e duty_u16(0-65535)",
        "   - escreve o teu próprio map: usa // e limita o resultado",
        "",
        "  Módulo 7: esperar sem bloquear a placa.",
    ],
    7: [
        "   - sleep deixa o chip surdo; ticks_ms deixa-o continuar a trabalhar",
        "   - if ticks_diff(ticks_ms(), last) >= period: last = ticks_add(last, period)",
        "   - o contador dá a volta; o período depende da plataforma, nunca compares com <",
        "",
        "  Módulo 8: código que corre no instante em que algo acontece.",
    ],
    8: [
        "   - pin.irq(handler=f, trigger=Pin.IRQ_FALLING) corre f no evento",
        "   - um handler é curto e não aloca nada; usa micropython.schedule",
        "   - protege dados partilhados de vários passos com disable_irq / enable_irq",
        "",
        "  Módulo 9: tornar o código rápido, com compilação nativa real.",
    ],
    9: [
        "   - @native e @viper compilam para código máquina; ambos correm neste PC",
        "   - viper precisa de tipos e dá acesso ptr8, sem verificar limites",
        "   - alocar num ciclo é o custo escondido habitual: floats, strings, listas",
        "   - const() não gasta RAM; gc.collect() antes de um ciclo sensível ao tempo",
        "",
        "  Módulo 10: assembly, para o próprio chip do Pico.",
    ],
    10: [
        "   - @micropython.asm_thumb monta código Cortex-M0+; até 4 args, r0 devolve",
        "   - r0-r7 estão livres; r8-r12 têm de ser repostos",
        "   - este PC não o corre; as listagens foram montadas com mpy-cross",
        "   - PIO é um assembly separado, em hardware, para sinais de tempo exato",
        "",
        "  Módulo 11: tudo junto, num robô.",
    ],
    11: [
        "   - divide o problema: ler o sensor, decidir, conduzir",
        "   - um modo mais ticks_ms é uma máquina de estados que nunca bloqueia",
        "   - um time_pulse_us negativo significa sem eco: longe, não zero",
        "   - funções pequenas e puras como choose_speed são fáceis de testar",
        "",
        "  É o curso. O curso de Arduino tem as mesmas ideias em C++,",
        "  até aos registos.",
    ],
}


def tr(text):
    return TEXT.get(text, text) if PT else text


def print_summary_pt(module):
    if not PT:
        return False
    for line in SUMMARIES[module]:
        print(line)
    return True
