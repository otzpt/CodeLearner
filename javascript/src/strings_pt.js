// Portuguese (PT-PT) text for the menus, prompts and module summaries.
// CODELEARNER_LANG=pt-pt turns it on. The lessons themselves stay in English.

const PT = process.env.CODELEARNER_LANG === "pt-pt";

const TEXT = {
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
  "JAVASCRIPT COURSE - FROM ZERO TO CLASSES": "CURSO DE JAVASCRIPT",
  "Running Node and console.log()": "Correr Node e console.log()",
  "Variables and types": "Variáveis e tipos",
  "Reading input": "Ler entrada",
  "Conditions": "Condições",
  "Loops": "Ciclos",
  "Arrays": "Arrays",
  "Functions and this": "Funções e this",
  "Objects": "Objetos",
  "Classes": "Classes",
  "Errors": "Erros",
  "Promises": "Promises",
  "Working with APIs": "Trabalhar com APIs",
  "Modules": "Módulos",
  "Node-specific stdlib": "Biblioteca própria do Node",
};

const SUMMARIES = {
  1: [
    "   - node file.js corre diretamente, sem compilação",
    "   - console.log() junta os argumentos com um espaço",
    "   - `${expr}` dentro de acentos graves interpola; as aspas normais não",
    "",
    "  Módulo 2: variáveis, e uma regra de coerção que o Python não tem.",
  ],
  2: [
    "   - sempre ===, nunca ==: o == converte com regras que ninguém decora",
    "   - NaN !== NaN; verifica com Number.isNaN()",
    "   - let/const controlam a reatribuição, não o tipo nem a mutabilidade",
  ],
  3: [
    "   - ler entrada é assíncrono; toda a cadeia de chamadas também tem de ser",
    "   - um terminal real funciona; um pipe de teste já fechado pode pendurar a segunda leitura",
    "   - o ask() deste curso evita isso percorrendo à mão o iterador do readline",
  ],
  4: [
    "   - só corre o primeiro ramo verdadeiro; a ordem importa",
    "   - [] e {} são VERDADEIROS em JS, ao contrário do Python",
    "   - = continua a não ser ==, e o Node nem avisa",
  ],
  5: [
    "   - for/while mantêm-se iguais, incluindo o erro de um a mais",
    "   - of -> valores, in -> chaves em string; não os confundas em arrays",
    "",
    "  Módulo 6: arrays, e o mesmo bug de aliasing do Python.",
  ],
  6: [
    "   - const impede reatribuir, não mutar; b = a continua a ser alias",
    "   - [...a] copia; sort() muda o array E ordena como texto por omissão",
    "   - map/filter/reduce devolvem algo novo; nenhum muda o original",
  ],
  7: [
    "   - função normal: this = o que estiver à esquerda do ponto ao chamar",
    "   - função seta: this = o que era no sítio onde a seta foi escrita",
    "   - let por iteração num ciclo; var partilha uma só ligação em todo ele",
    "",
    "  Módulo 8: objetos, e o único caso em que == e === estão ambos errados.",
  ],
  8: [
    "   - == e === comparam objetos/arrays por referência, não por conteúdo",
    "   - JSON.stringify/.parse fazem os dados ir e voltar, e comparam o conteúdo indiretamente",
    "   - ?. para num elo em falta em vez de lançar erro",
  ],
  9: [
    "   - #campo é privado a sério: SyntaxError de fora, não uma convenção",
    "   - extends/super funcionam como a herança do C++",
    "   - não há momento garantido para o destrutor: usa try/finally para limpar com certeza",
  ],
  10: [
    "   - throw para assinalar um problema; try/catch para o tratar; finally corre sempre",
    "   - TypeError, RangeError, SyntaxError: sabe o que cada um quer dizer",
    "   - extends Error + super(msg) é como crias o teu próprio tipo apanhável",
    "",
    "  Fica assim o curso principal. Como em Python, sem memória manual:",
    "  coletor de lixo, com try/finally no lugar do RAII.",
    "",
    "  O módulo 11 dá nome ao objeto que o await anda a desembrulhar",
    "  desde o módulo 3: uma Promise.",
  ],
  11: [
    "   - uma Promise está pendente, e depois resolve-se uma vez, cumprida ou rejeitada",
    "   - await desembrulha esse mesmo objeto; .then()/.catch() é a",
    "     sintaxe antiga para a mesma coisa",
    "   - uma rejeição com await lança erro, apanhável com try/catch normal",
    "   - Promise.all corre awaits independentes em simultâneo em vez",
    "     de um a seguir ao outro",
    "",
    "  O módulo 12 é trabalhar com uma API real: fetch e JSON, ambos",
    "  já utilizáveis com tudo o que foi visto aqui.",
  ],
  12: [
    "   - fetch(url) resolve para uma Response, não para o corpo já lido:",
    "     response.json() é uma segunda Promise, separada",
    "   - fetch só rejeita numa falha real de rede (DNS, ligação",
    "     recusada, timeout), nunca num 404 ou 500",
    "   - um 404/500 é uma troca concluída com sucesso; verificar",
    "     response.ok ou response.status é trabalho de quem chama",
    "   - try/catch (módulo 10) continua a apanhar as falhas que são",
    "     mesmo rejeições, sem mecanismo novo",
  ],
  13: [
    "   - require(caminho) corre esse ficheiro uma vez e devolve o que ele",
    "     atribuiu a module.exports; o ui.js deste curso e todos os",
    "     lessons_*.js já funcionam exatamente assim",
    "   - um nome simples no require() procura um pacote instalado; os teus",
    "     ficheiros precisam de um caminho explícito ./ ou ../",
    "   - import/export é um segundo sistema de módulos, normalizado, que o",
    "     Node também suporta: um .mjs (ou \"type\": \"module\" no package.json) é ESM",
    "   - os dois não se misturam no mesmo ficheiro: um import faz o Node",
    "     tratar o ficheiro TODO como ESM, onde require nem sequer existe",
  ],
  14: [
    "   - fs.readFileSync/writeFileSync bloqueiam o processo até terminar:",
    "     aceitável numa ferramenta CLI curta, em mais nenhum caso",
    "   - fs.promises.readFile/writeFile devolvem Promises, o mesmo padrão",
    "     de await dos módulos 11 e 12",
    "   - process.argv[0] é o executável do node, [1] é o script, e os teus",
    "     argumentos começam no índice 2",
    "   - acesso direto ao sistema de ficheiros e o objeto process são",
    "     coisas que o JavaScript de um navegador nunca tem",
  ],
};

function tr(text) {
  return PT && TEXT[text] !== undefined ? TEXT[text] : text;
}

function printSummaryPt(module) {
  if (!PT) {
    return false;
  }
  for (const line of SUMMARIES[module]) {
    console.log(line);
  }
  return true;
}

module.exports = { tr, printSummaryPt };
