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
    "PYTHON COURSE - FROM ZERO TO CLASSES": "CURSO DE PYTHON",
    "Running Python and print()": "Correr Python e print()",
    "Variables and types": "Variáveis e tipos",
    "Reading input": "Ler entrada",
    "Conditions": "Condições",
    "Loops": "Ciclos",
    "Lists": "Listas",
    "Functions and default arguments": "Funções e argumentos por omissão",
    "Dictionaries": "Dicionários",
    "Classes": "Classes",
    "Exceptions": "Exceções",
    "Modules and packages": "Módulos e pacotes",
    "Files and context managers": "Ficheiros e gestores de contexto",
    "Working with APIs": "Trabalhar com APIs",
}

SUMMARIES = {
    1: [
        "   - python3 file.py corre diretamente, sem compilação",
        "   - print() junta os argumentos com um espaço; sep= e end= mudam isso",
        "   - um erro mais à frente no ficheiro só é apanhado quando a execução lá chega",
        "",
        "  Módulo 2: variáveis, e uma regra de divisão que não é a do C.",
    ],
    2: [
        "   - atribuição, não declaração; um nome pode apontar para qualquer tipo",
        "   - / é sempre divisão real; // arredonda para baixo, em direção a menos infinito",
        "   - f-strings: f\"{expr:spec}\", calculado uma vez, formatado para mostrar",
    ],
    3: [
        "   - input() devolve sempre str; converte com int()/float()",
        "   - str + int lança TypeError em vez de fazer em silêncio a coisa errada",
        "   - int(texto_mau) lança ValueError, que se apanha com try/except",
    ],
    4: [
        "   - a indentação É o bloco; sem chavetas, e a inconsistência é erro de sintaxe",
        "   - 0, 0.0, \"\", [], {}, None são falsos; tudo o resto é verdadeiro",
        "   - `if name:` é a forma idiomática de testar se está vazio",
    ],
    5: [
        "   - for percorre uma sequência; range(a, b) vai de a até b-1",
        "   - percorrer uma lista não precisa de índice; enumerate() acrescenta um",
        "   - n -= 1, não n--; break/continue são iguais ao C",
        "",
        "  Módulo 6: listas, e o bug de aliasing que custa uma tarde.",
    ],
    6: [
        "   - b = a é um alias; a[:] / list(a) / a.copy() copiam mesmo",
        "   - o fim de um slice é exclusivo, a mesma regra de range()",
        "   - sort() devolve None; sorted() devolve uma lista nova",
    ],
    7: [
        "   - um valor por omissão é calculado uma vez, na definição",
        "   - nunca uses um valor mutável por omissão; usa None e constrói lá dentro",
        "   - *args -> tuplo dos extras, **kwargs -> dicionário dos extras",
    ],
    8: [
        "   - [] lança KeyError; .get(chave, omissão) nunca lança",
        "   - a ordem de inserção mantém-se, desde a 3.7, mas não fica ordenado",
        "   - setdefault(chave, []).append(x) é a forma habitual de agrupar",
    ],
    9: [
        "   - self é explícito; __init__ é o construtor",
        "   - não há privacidade imposta: um _ inicial é convenção, não fechadura",
        "   - não há momento garantido para o destrutor: usa `with` para limpar com certeza",
    ],
    10: [
        "   - raise para assinalar um problema; try/except para o tratar; finally corre sempre",
        "   - apanha um tipo específico; um except sem tipo esconde bugs, incluindo os teus",
        "   - ValueError, TypeError, KeyError, IndexError: sabe o que cada um quer dizer",
        "",
        "  Isto cobre um programa em execução. O que em C exigia memória",
        "  manual, e o C++ liga ao âmbito, o Python deixa ao coletor de",
        "  lixo, e dá-te em troca exceções e `with`.",
        "",
        "  O módulo 11 trata da outra metade: trabalhar com mais do que",
        "  um ficheiro.",
    ],
    11: [
        "   - import traz um módulo; from X import Y dispensa o prefixo X.",
        "   - cada módulo guarda o seu nome em __name__",
        "   - __name__ == \"__main__\" só é verdadeiro no ficheiro executado",
        "   - um pacote é uma pasta com __init__.py; pip/venv é como um",
        "     projeto real vai buscar código que não escreveu",
        "",
        "  O módulo 12 é a outra metade de trabalhar com ficheiros: lê-los",
        "  e escrevê-los, e o `with` para onde os módulos 6 a 10 já",
        "  estavam a apontar.",
    ],
    12: [
        "   - open()/.write()/.close() funcionam, mas só se o close() correr",
        "     mesmo: esquecê-lo, ou lançar antes, deixa o ficheiro aberto",
        "   - with open(...) as f: fecha o ficheiro à saída, com ou sem",
        "     exceção, provado acima e não apenas afirmado",
        "   - .readlines() mantém cada \\n; .write() nunca acrescenta nenhum",
        "   - json.dump/json.load fazem um valor ir e voltar por um ficheiro;",
        "     json.dumps/loads fazem o mesmo por uma string",
        "",
        "  O módulo 9 disse que o momento da recolha de lixo nunca foi uma",
        "  garantia em que te pudesses apoiar. Esta é a garantia que o",
        "  Python dá em vez disso, e foi preciso um ficheiro real e uma",
        "  exceção real para a mostrar.",
    ],
    13: [
        "   - urllib.request.urlopen(url, timeout=...) abre uma ligação HTTP",
        "     real; a resposta funciona como os ficheiros do módulo 12:",
        "     .read() devolve bytes, decode() + json.loads() dá um dicionário",
        "   - a armadilha: urlopen LANÇA urllib.error.HTTPError num estado",
        "     4xx/5xx, ao contrário do fetch de JS, que resolve e deixa a",
        "     verificação de response.ok para quem chama",
        "   - urllib.error.URLError cobre uma falha real de rede (DNS, ligação",
        "     recusada, timeout); HTTPError é um URLError, por isso tem de",
        "     ser apanhado primeiro ou o seu except nunca corre",
        "   - timeout=... em cada chamada real impede que uma rede morta",
        "     pendure a lição à espera em silêncio",
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
