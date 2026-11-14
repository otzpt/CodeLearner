module.exports = {
  id: "path-traversal",
  title: "Path traversal",
  titlePt: "Fuga de pasta",
  screens: [
    [
      "A program that serves files from one folder often takes the file name from the user. .. in a path means \"the folder above\", so a name like ../secret.txt climbs out of the folder:",
      { code: 'import os\n\nos.makedirs("public", exist_ok=True)\nwith open("public/hello.txt", "w") as f:\n    f.write("public file")\nwith open("secret.txt", "w") as f:\n    f.write("the admin password")\n\ndef serve(name):\n    with open(os.path.join("public", name)) as f:\n        return f.read()\n\nprint(serve("hello.txt"))\nprint(serve("../secret.txt"))' },
      "The user asked for a file in public and got one outside it.",
    ],
    [
      "The fix: work out the real, full path, and refuse anything that is not inside the folder. os.path.realpath resolves .. and links:",
      { code: 'import os\n\nos.makedirs("public", exist_ok=True)\nwith open("public/hello.txt", "w") as f:\n    f.write("public file")\n\nBASE = os.path.realpath("public")\n\ndef serve(name):\n    path = os.path.realpath(os.path.join(BASE, name))\n    if os.path.commonpath([BASE, path]) != BASE:\n        return "refused"\n    with open(path) as f:\n        return f.read()\n\nprint(serve("hello.txt"))\nprint(serve("../secret.txt"))' },
      "commonpath gives the longest folder both paths share. If that is not BASE, the file is outside.",
      { quiz: "What does .. mean in a path?", choices: ["the current folder", "the folder above"], answer: 2, why: ".. climbs one level up." },
    ],
    [
      {
        exercise: "paths",
        task: 'Write is_inside(base, name) that returns True only if base joined with name, resolved with os.path.realpath, stays inside realpath(base). Print is_inside("docs", "a.txt"), is_inside("docs", "../etc/passwd") and is_inside("docs", "sub/../b.txt").',
        expected: ["True", "False", "True"],
        hints: ["base = os.path.realpath(base); path = os.path.realpath(os.path.join(base, name))", "return os.path.commonpath([base, path]) == base"],
        solution: 'import os\n\ndef is_inside(base, name):\n    base = os.path.realpath(base)\n    path = os.path.realpath(os.path.join(base, name))\n    return os.path.commonpath([base, path]) == base\n\nprint(is_inside("docs", "a.txt"))\nprint(is_inside("docs", "../etc/passwd"))\nprint(is_inside("docs", "sub/../b.txt"))',
      },
    ],
  ],
  recapPt: [
    "Nomes de ficheiro com .. podem sair da pasta pretendida.",
    "Resolve o caminho real com os.path.realpath.",
    "Recusa tudo o que não fique dentro da pasta (os.path.commonpath).",
  ],
  recap: [
    "File names with .. can climb out of the intended folder.",
    "Resolve the real path with os.path.realpath.",
    "Refuse anything that is not inside the folder (os.path.commonpath).",
  ],
};
