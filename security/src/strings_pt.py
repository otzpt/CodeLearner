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
    "SECURITY WITH PYTHON": "SEGURANCA COM PYTHON",
    "Your first request": "O teu primeiro pedido",
    "Plain HTTP is readable on the wire": "O HTTP simples lê-se na ligação",
}

SUMMARIES = {
    1: [
        "   - uma resposta HTTP é uma linha de estado, cabeçalhos e corpo;",
        "     a segurança lê-se nos dois primeiros, não só no corpo",
        "   - o cabeçalho Server revela informação: um número de versão é",
        "     o primeiro reconhecimento do atacante, oferecido de graça",
        "   - os cabeçalhos de segurança são opcionais; o achado é o que",
        "     FALTA, visível só porque o procuraste de propósito",
        "   - urllib LANÇA em 4xx/5xx (HTTPError, subclasse de URLError);",
        "     nunca deixes a exceção passar pelo resultado real",
        "   - passa sempre timeout=: o primeiro script de rede sem timeout",
        "     é a clássica tarde de 'porque é que isto congelou'",
    ],
    2: [
        "   - cada máquina no caminho recebe os bytes que envias",
        "   - em HTTP simples esses bytes incluem a palavra-passe, em texto",
        "   - em HTTPS a captura não contém nem a palavra-passe nem o caminho",
        "   - o nome do servidor continua visível no handshake TLS",
        "   - captura só o teu próprio tráfego, na tua própria máquina",
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
