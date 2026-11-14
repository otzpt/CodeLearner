// How the Java course runs code: java runs a single source file directly
// (JDK 11 or later), compiling it in memory first.

module.exports = {
  id: "java",
  title: "JAVA FROM ZERO",
  titlePt: "JAVA DO ZERO",
  file: "Main.java",
  starter: "public class Main {\n    public static void main(String[] args) {\n        // your code here\n    }\n}",
  steps: (file) => [["java", file]],
  howToRun: (name) => `java ${name}`,

  // Compile errors: "File.java:LINE: error: message". Runtime: an
  // "Exception in thread" line, then "at Main.main(File.java:LINE)".
  parseError(text) {
    const c = text.match(/\.java:(\d+): error: (.*)/);
    if (c) {
      return { errorLine: "error: " + c[2], line: Number(c[1]) };
    }
    const ex = text.match(/Exception in thread "main" (?:java\.\w+\.)*(\S+?)(?::\s*(.*))?$/m);
    const where = text.match(/\.java:(\d+)\)/);
    if (ex) {
      return { errorLine: ex[1] + (ex[2] ? ": " + ex[2] : ""), line: where ? Number(where[1]) : null };
    }
    return { errorLine: text.trim().split("\n")[0] || "" };
  },

  errors: [
    [/cannot find symbol/, "a name Java does not know: a typo (capitals count), or a variable used outside the block it was made in.", "um nome que o Java não conhece: um erro de escrita (as maiúsculas contam), ou uma variável usada fora do bloco onde foi criada."],
    [/';' expected/, "a missing semicolon at the end of the line named.", "falta um ponto e vírgula no fim da linha indicada."],
    [/expected/, "something is typed in a way Java cannot read: often a missing ; ) or }.", "algo está escrito de uma forma que o Java não consegue ler: muitas vezes falta ; ) ou }."],
    [/incompatible types|possible lossy conversion/, "a value of one type where another is needed.", "um valor de um tipo onde é preciso outro."],
    [/ArrayIndexOutOfBounds|IndexOutOfBounds/, "a position outside the array or list. Positions go from 0 to length - 1.", "uma posição fora do array ou lista. As posições vão de 0 a length - 1."],
    [/NullPointerException/, "a variable that holds null (no object) was used as if it held one.", "uma variável com null (nenhum objeto) foi usada como se tivesse um."],
    [/ArithmeticException/, "a whole-number division by zero.", "uma divisão inteira por zero."],
    [/NumberFormatException/, "text that is not a number was turned into a number.", "texto que não é um número foi convertido para número."],
    [/InputMismatchException/, "the user typed something that does not fit the type being read.", "o utilizador escreveu algo que não serve para o tipo que estava a ser lido."],
    [/FileNotFoundException|NoSuchFileException/, "the file does not exist where the program looked.", "o ficheiro não existe onde o programa procurou."],
    [/^\w+Exception/, "an exception was thrown and nothing caught it, so the program stopped.", "foi lançada uma exceção e nada a apanhou, por isso o programa parou."],
  ],
};
