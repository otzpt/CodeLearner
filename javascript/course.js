// How the JavaScript course runs code: one file, run with node.

module.exports = {
  id: "javascript",
  title: "JAVASCRIPT FROM ZERO",
  titlePt: "JAVASCRIPT DO ZERO",
  file: "main.js",
  steps: (file) => [["node", file]],
  howToRun: (name) => `node ${name}`,

  // Node prints the file and line, the code, a caret, then "XError: ...".
  parseError(stderr) {
    const errorLine = stderr.split("\n").find((l) => /^\w*Error\b/.test(l)) || stderr.trim().split("\n")[0] || "";
    const where = stderr.match(/\.js:(\d+)/);
    return { errorLine, line: where ? Number(where[1]) : null };
  },

  errors: [
    [/^ReferenceError/, "a name that does not exist. Check the spelling and capitals.", "um nome que não existe. Verifica a ortografia e as maiúsculas."],
    [/^SyntaxError/, "code JavaScript cannot read: often a missing quote, bracket or brace.", "código que o JavaScript não consegue ler: muitas vezes falta uma aspa, um parêntese ou uma chaveta."],
    [/^TypeError/, "a value used in a way it does not allow.", "um valor usado de uma forma que não permite."],
    [/^RangeError/, "a number outside what is allowed.", "um número fora do que é permitido."],
  ],
};
