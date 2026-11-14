// How the C course runs code: compile with the system C compiler, with
// warnings on, then run the program.

const WINDOWS = process.platform === "win32";
const CC = process.env.CC || (WINDOWS ? "gcc" : "cc");
const PROGRAM = WINDOWS ? "program.exe" : "./program";

const STARTER = `#include <stdio.h>

int main(void)
{
    // your code here
    return 0;
}`;

module.exports = {
  id: "c",
  title: "C FROM ZERO",
  titlePt: "C DO ZERO",
  file: "main.c",
  starter: STARTER,
  steps: (file, dir, opts = {}) => [
    [CC, "-std=c17", "-Wall", "-Wextra", ...(opts.flags || []), "-o", WINDOWS ? "program.exe" : "program", file, "-lm"],
    [PROGRAM],
  ],
  howToRun: (name) => `${CC} -Wall -Wextra -o program ${name} && ${PROGRAM}`,

  // The compiler writes "file.c:LINE:COL: error: message". AddressSanitizer
  // reports "ERROR: AddressSanitizer: kind ..." and a stack with file:line.
  parseError(text) {
    const asan = text.match(/(AddressSanitizer|LeakSanitizer): ([\w-]+)/);
    if (asan) {
      const where = text.match(/main\.c:(\d+)/) || text.match(/\.c:(\d+)/);
      return { errorLine: `${asan[1]}: ${asan[2]}`, line: where ? Number(where[1]) : null };
    }
    const m = text.match(/:(\d+):\d+: (?:fatal )?error: (.*)/);
    if (m) {
      return { errorLine: "error: " + m[2], line: Number(m[1]) };
    }
    const link = text.split("\n").find((l) => /undefined reference|ld returned/.test(l));
    return { errorLine: link ? link.replace(/^.*?: /, "") : text.trim().split("\n")[0] || "" };
  },

  warnings: (text) =>
    [...text.matchAll(/:(\d+):\d+: warning: (.*)/g)].map((m) => `warning (line ${m[1]}): ${m[2]}`),

  errors: [
    [/buffer-overflow/, "the program read or wrote past the end of an array.", "o programa leu ou escreveu para lá do fim de um array."],
    [/use-after-free/, "the program used memory after giving it back with free.", "o programa usou memória depois de a devolver com free."],
    [/double-free/, "the same memory was given back with free twice.", "a mesma memória foi devolvida com free duas vezes."],
    [/LeakSanitizer|detected memory leaks/, "memory from malloc was never given back with free.", "memória do malloc nunca foi devolvida com free."],
    [/undeclared|implicit declaration/, "a name the compiler does not know: a typo, a variable used before it was declared, or a missing #include.", "um nome que o compilador não conhece: um erro de escrita, uma variável usada antes de ser declarada, ou um #include em falta."],
    [/expected ';'/, "a missing semicolon. Every statement ends with ;, usually on the line before the one named.", "falta um ponto e vírgula. Cada instrução acaba em ;, normalmente na linha antes da indicada."],
    [/expected/, "something is typed in a way C cannot read: often a missing ; ) or }.", "algo está escrito de uma forma que o C não consegue ler: muitas vezes falta ; ) ou }."],
    [/undefined reference to .main/, "the program has no main function, so there is nowhere to start.", "o programa não tem a função main, por isso não há onde começar."],
    [/undefined reference/, "a function that was declared but never written, or a misspelled function name.", "uma função declarada mas nunca escrita, ou um nome de função mal escrito."],
    [/incompatible|makes integer from pointer|makes pointer from integer/, "a value of one type where another type is needed, often a missing & or *.", "um valor de um tipo onde é preciso outro, muitas vezes falta um & ou *."],
  ],
};
