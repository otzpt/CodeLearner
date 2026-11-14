// How the Rust course runs code: compile with rustc, then run the program.

const WINDOWS = process.platform === "win32";
const PROGRAM = WINDOWS ? "program.exe" : "./program";

module.exports = {
  id: "rust",
  title: "RUST FROM ZERO",
  titlePt: "RUST DO ZERO",
  file: "main.rs",
  starter: "fn main() {\n    // your code here\n}",
  steps: (file) => [
    ["rustc", "--edition", "2021", "--crate-name", "main", "-o", WINDOWS ? "program.exe" : "program", file],
    [PROGRAM],
  ],
  howToRun: (name) => `rustc --crate-name main -o program ${name} && ${PROGRAM}`,

  // Compile: "error[Ennnn]: message" then " --> file.rs:LINE:COL".
  // Runtime: "thread 'main' panicked at file.rs:LINE:COL:" then the message.
  parseError(text) {
    const c = text.match(/^error(?:\[(E\d+)\])?: (.*)\n\s*--> [^:]+:(\d+)/m);
    if (c) {
      return { errorLine: `error${c[1] ? "[" + c[1] + "]" : ""}: ${c[2]}`, line: Number(c[3]) };
    }
    const p = text.match(/panicked at [^:]+:(\d+):\d+:\n(.*)/);
    if (p) {
      return { errorLine: "panic: " + p[2], line: Number(p[1]) };
    }
    const first = text.split("\n").find((l) => /^error/.test(l));
    return { errorLine: first || "" };
  },

  warnings: (text) =>
    [...text.matchAll(/^warning: (.*)\n\s*--> [^:]+:(\d+)/gm)].map((m) => `warning (line ${m[2]}): ${m[1]}`),

  errors: [
    [/E0425|E0433|E0412/, "a name Rust does not know: a typo, or something used outside the block it was made in.", "um nome que o Rust não conhece: um erro de escrita, ou algo usado fora do bloco onde foi criado."],
    [/E0384|cannot assign twice/, "a variable without mut was given a new value. Add mut where it is created: let mut x.", "uma variável sem mut recebeu um valor novo. Junta mut onde é criada: let mut x."],
    [/E0382|use of moved value|borrow of moved value/, "the value was moved somewhere else and this name no longer owns it. Borrow it with & or make a copy with .clone().", "o valor foi movido para outro lado e este nome já não é o dono. Empresta-o com & ou faz uma cópia com .clone()."],
    [/E0502|E0499|E0596|borrow/, "a borrowing rule was broken: one &mut at a time, or any number of & but no &mut alongside.", "uma regra de empréstimo foi quebrada: um &mut de cada vez, ou vários & mas nenhum &mut ao mesmo tempo."],
    [/E0308|mismatched types/, "a value of one type where another is needed.", "um valor de um tipo onde é preciso outro."],
    [/expected `;`|expected one of/, "something is typed in a way Rust cannot read: often a missing ; ) or }.", "algo está escrito de uma forma que o Rust não consegue ler: muitas vezes falta ; ) ou }."],
    [/index out of bounds/, "a position outside the vector. Positions go from 0 to len - 1.", "uma posição fora do vetor. As posições vão de 0 a len - 1."],
    [/overflow/, "a number went past the biggest (or smallest) value its type can hold.", "um número passou do maior (ou menor) valor que o seu tipo aguenta."],
    [/called `(Option|Result)::unwrap\(\)`/, "unwrap() met a None or an Err and stopped the program.", "unwrap() encontrou um None ou um Err e parou o programa."],
    [/divide by zero/, "a whole-number division by zero.", "uma divisão inteira por zero."],
    [/^panic/, "the program hit a problem it could not continue from, and stopped.", "o programa encontrou um problema do qual não podia continuar, e parou."],
  ],
};
