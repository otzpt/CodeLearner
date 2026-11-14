// Portuguese (PT-PT) for the menus, prompts, lesson titles and summaries.
// CODELEARNER_LANG=pt-pt turns it on. The lesson text stays in English, and
// every piece of code stays exactly as it is.

const PT = process.env.CODELEARNER_LANG === "pt-pt";

const TEXT = {
  // main.js
  "JAVASCRIPT FROM ZERO": "JAVASCRIPT DO ZERO",
  "No experience needed. Each lesson explains one idea, runs real examples, and ends with a small program you write yourself and the course checks.":
    "Não precisas de experiência. Cada lição explica uma ideia, corre exemplos reais e acaba com um pequeno programa que escreves tu e o curso verifica.",
  "  <- next": "  <- seguinte",
  "Your practice files go in": "Os teus ficheiros de prática ficam em",
  "  Lesson number, [ENTER] for the next one, [q] quit: ": "  Número da lição, [ENTER] para a seguinte, [q] Sair: ",
  "  Lesson number, [q] quit: ": "  Número da lição, [q] Sair: ",
  "See you next time.": "Até à próxima.",
  "Something went wrong:": "Algo correu mal:",

  // app.js: screens
  LESSON: "LIÇÃO",
  "WHAT YOU LEARNED": "RESUMO",
  "  [ENTER] back to the lessons: ": "  [ENTER] voltar às lições: ",
  "  [ENTER] next   [b] back   [m] menu: ": "  [ENTER] seguinte   [b] anterior   [m] menu: ",
  "Remember: ": "Lembra-te: ",

  // app.js: examples and errors
  "Output:": "Resultado:",
  "Run, typing:": "Corrido a escrever:",
  "(the ask helper goes here)": "(aqui vai a função ask)",
  "What this means: ": "O que isto quer dizer: ",
  "a name that does not exist. Check the spelling and capitals.": "um nome que não existe. Verifica a ortografia e as maiúsculas.",
  "code JavaScript cannot read: often a missing quote, bracket or brace.": "código que o JavaScript não consegue ler: muitas vezes falta uma aspa, um parêntese ou uma chaveta.",
  "a value used in a way it does not allow.": "um valor usado de uma forma que não permite.",
  "a number outside what is allowed.": "um número fora do que é permitido.",
  "read the message: it names the problem and the line.": "lê a mensagem: diz qual é o problema e a linha.",
  line: "linha",

  // app.js: quizzes
  "Quick question: ": "Pergunta rápida: ",
  "  Your answer (number): ": "  A tua resposta (número): ",
  "Right. ": "Certo. ",
  "Not quite. ": "Não é bem. ",
  "It is": "É a",
  "Not quite.": "Não é bem.",
  " Have another go.": " Tenta outra vez.",

  // app.js: exercises
  "YOUR TURN": "A TUA VEZ",
  "  When the checker runs it, it types: ": "  Quando o verificador o corre, escreve: ",
  "  It must print:": "  Tem de mostrar:",
  "  Write it in ": "  Escreve-o em ",
  " (open it in any text editor).": " (abre-o num editor de texto qualquer).",
  "  [ENTER] check my file   [h] hint   [s] show a solution   [k] skip: ": "  [ENTER] verificar o ficheiro   [h] dica   [s] ver uma solução   [k] saltar: ",
  "Hint: ": "Dica: ",
  "  One way to write it (yours can look different and still be right):": "  Uma forma de o escrever (o teu pode ser diferente e estar certo):",
  "Your program did not finish within 5 seconds.": "O teu programa não terminou em 5 segundos.",
  " Is there a loop that never stops?": " Há algum ciclo que nunca para?",
  "Your program stopped with an error:": "O teu programa parou com um erro:",
  "Correct! ": "Certo! ",
  "Your program printed exactly what was asked.": "O teu programa mostrou exatamente o que era pedido.",
  "Not yet.": "Ainda não.",
  " Your program printed:": " O teu programa mostrou:",
  "(nothing)": "(nada)",
  "should be:": "devia ser:",
  "  The asked lines are all there, but there are extra lines after them.": "  As linhas pedidas estão todas lá, mas há linhas a mais depois delas.",
};

function tr(text) {
  return PT && TEXT[text] !== undefined ? TEXT[text] : text;
}

module.exports = { PT, tr };
