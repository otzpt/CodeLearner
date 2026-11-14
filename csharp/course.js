// How the C# course runs code: dotnet run on a single .cs file (.NET 10 or
// later), which builds and runs it with no project file.

module.exports = {
  id: "csharp",
  title: "C# FROM ZERO",
  titlePt: "C# DO ZERO",
  file: "main.cs",
  starter: 'Console.WriteLine("Hello!"); // replace this line with your code',
  steps: (file) => [["dotnet", "run", file]],
  timeout: 60000,
  // Invariant culture: decimals print as 3.4 whatever the machine's
  // language, so the checker and the lessons agree.
  env: { DOTNET_CLI_TELEMETRY_OPTOUT: "1", DOTNET_NOLOGO: "1", DOTNET_SYSTEM_GLOBALIZATION_INVARIANT: "1" },
  errorsOnStdout: true,
  stdoutWarning: /^\S+\.cs\((\d+),\d+\): warning (CS\d+: .*?)(?: \[.*\])?$/,
  howToRun: (name) => `dotnet run ${name}`,

  // Build errors: "file.cs(LINE,COL): error CSnnnn: message". Runtime:
  // "Unhandled exception. System.Type: message" and "in file.cs:line N".
  parseError(text) {
    const b = text.match(/\.cs\((\d+),\d+\): error (CS\d+): (.*?)(?: \[.*\])?$/m);
    if (b) {
      return { errorLine: `error ${b[2]}: ${b[3]}`, line: Number(b[1]), compile: true };
    }
    const ex = text.match(/Unhandled exception\. (?:System\.(?:\w+\.)*)?(\w+): (.*)/);
    const where = text.match(/\.cs:line (\d+)/);
    if (ex) {
      return { errorLine: `${ex[1]}: ${ex[2]}`, line: where ? Number(where[1]) : null };
    }
    return { errorLine: "" };
  },

  errors: [
    [/CS0103|CS0246|CS1061/, "a name C# does not know: a typo (capitals count), or something used before or outside where it was made.", "um nome que o C# não conhece: um erro de escrita (as maiúsculas contam), ou algo usado antes ou fora de onde foi criado."],
    [/CS1002/, "a missing semicolon at the end of the line named.", "falta um ponto e vírgula no fim da linha indicada."],
    [/CS1003|CS1026|CS1513|CS1514/, "something is typed in a way C# cannot read: often a missing ; ) or }.", "algo está escrito de uma forma que o C# não consegue ler: muitas vezes falta ; ) ou }."],
    [/CS0029|CS0266|CS1503/, "a value of one type where another is needed.", "um valor de um tipo onde é preciso outro."],
    [/CS0122/, "that member is private: only the class's own code may use it.", "esse membro é privado: só o código da própria classe o pode usar."],
    [/CS0535/, "the class promised an interface but did not write all of its methods.", "a classe prometeu uma interface mas não escreveu todos os seus métodos."],
    [/IndexOutOfRange|ArgumentOutOfRange/, "a position outside the array or list. Positions go from 0 to length - 1.", "uma posição fora do array ou lista. As posições vão de 0 a length - 1."],
    [/NullReference/, "a variable holding null (no object) was used as if it held one.", "uma variável com null (nenhum objeto) foi usada como se tivesse um."],
    [/DivideByZero/, "a whole-number division by zero.", "uma divisão inteira por zero."],
    [/FormatException/, "text that is not a number was turned into a number.", "texto que não é um número foi convertido para número."],
    [/FileNotFound/, "the file does not exist where the program looked.", "o ficheiro não existe onde o programa procurou."],
    [/Exception/, "an exception was thrown and nothing caught it, so the program stopped.", "foi lançada uma exceção e nada a apanhou, por isso o programa parou."],
  ],
};
