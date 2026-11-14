// Portuguese (PT-PT) for the engine's menus, prompts and feedback. Lesson
// titles and summaries are translated in each lesson (titlePt, recapPt).
// CODELEARNER_LANG=pt-pt turns it on. The lesson text stays in English, and
// every piece of code stays exactly as it is.

const PT = process.env.CODELEARNER_LANG === "pt-pt";

const TEXT = {
  // main.js
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
  "What this means: ": "O que isto quer dizer: ",
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
  "Your program did not finish within 10 seconds.": "O teu programa não terminou em 10 segundos.",
  " Is there a loop that never stops, or is it waiting for more input?": " Há algum ciclo que nunca para, ou está à espera de mais entrada?",
  "Your program did not build:": "O teu programa não compilou:",
  " (what was typed is not shown)": " (o que foi escrito não aparece)",
  "  It must look like this (you type the bold parts):": "  Tem de ficar assim (tu escreves as partes a negrito):",
  "To run it yourself:": "Para o correres tu:",
  "not installed": "não está instalado",
  "Builds without errors.": "Compila sem erros.",
  "Run it to open the window.": "Corre-o para abrir a janela.",
  "It builds! ": "Compila! ",
  "Now run it and check that the window does what the task asks.": "Agora corre-o e confirma que a janela faz o que a tarefa pede.",
  "exited with code": "terminou com o código",
  "(the helper above goes here)": "(aqui vai o código de apoio do ecrã anterior)",
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
