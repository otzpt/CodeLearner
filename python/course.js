// How the Python course runs code: one file, run with python3 (python on
// Windows, where the installer does not add python3).

const PYTHON = process.platform === "win32" ? "python" : "python3";

module.exports = {
  id: "python",
  title: "PYTHON FROM ZERO",
  titlePt: "PYTHON DO ZERO",
  file: "main.py",
  steps: (file) => [[PYTHON, file]],
  env: { PYTHONDONTWRITEBYTECODE: "1", PYTHONIOENCODING: "utf-8" },
  howToRun: (name) => `${PYTHON} ${name}`,

  // A traceback ends with "SomeError: message"; the last "line N" in it is
  // where it happened.
  parseError(stderr) {
    const lines = stderr.trim().split("\n");
    const errorLine = lines.reverse().find((l) => /^\w+(Error|Exception)\b|^KeyboardInterrupt/.test(l)) || lines[0] || "";
    const all = [...stderr.matchAll(/line (\d+)/g)];
    return { errorLine, line: all.length ? Number(all[all.length - 1][1]) : null };
  },

  errors: [
    [/^NameError/, "a name that does not exist yet. Check the spelling and capitals, and that it was given a value above this line.", "um nome que ainda não existe. Verifica a ortografia e as maiúsculas, e se recebeu um valor antes desta linha."],
    [/^IndentationError/, "the spaces at the start of a line are wrong. Lines in the same block must start at the same column.", "os espaços no início de uma linha estão errados. As linhas do mesmo bloco têm de começar na mesma coluna."],
    [/^SyntaxError/, "code Python cannot read: often a missing quote, bracket or colon.", "código que o Python não consegue ler: muitas vezes falta uma aspa, um parêntese ou dois pontos."],
    [/^TypeError/, "a value used in a way its type does not allow, like adding text to a number.", "um valor usado de uma forma que o seu tipo não permite, como somar texto a um número."],
    [/^ValueError/, "the right type, but a value that does not make sense, like int(\"hello\").", "o tipo certo, mas um valor que não faz sentido, como int(\"hello\")."],
    [/^ZeroDivisionError/, "a division by zero.", "uma divisão por zero."],
    [/^IndexError/, "a position that is not in the list or string. Positions go from 0 to length - 1.", "uma posição que não existe na lista ou string. As posições vão de 0 a length - 1."],
    [/^KeyError/, "a key that is not in the dictionary.", "uma chave que não está no dicionário."],
    [/^AttributeError/, "a name after a dot that this value does not have.", "um nome depois de um ponto que este valor não tem."],
    [/^FileNotFoundError/, "the file does not exist where the program looked for it.", "o ficheiro não existe onde o programa o procurou."],
  ],
};
