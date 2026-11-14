module.exports = {
  id: "ignore-and-tags",
  title: "Ignoring files and tagging versions",
  titlePt: "Ignorar ficheiros e marcar versões",
  screens: [
    [
      "Some files should never be in the history: build output, logs, passwords. List their names or patterns in a file called .gitignore and Git stops showing them. * matches any characters:",
      { code: 'git init -q\nprintf "*.log\\nsecret.txt\\n" > .gitignore\necho "data" > app.txt\necho "debug" > run.log\necho "pass" > secret.txt\ngit status --short' },
      ".gitignore itself should be committed, so everyone shares the same list.",
    ],
    [
      "A tag gives a commit a permanent name, usually a version number. git tag lists them, and the tag can be used wherever a commit is expected:",
      { code: 'git init -q\necho "1.0" > version.txt\ngit add version.txt\ngit commit -q -m "Release 1.0"\ngit tag v1.0\necho "1.1" > version.txt\ngit commit -q -am "Work on 1.1"\ngit tag\ngit show v1.0:version.txt' },
      { quiz: "Where do you list files Git should not track?", choices: ["in .gitignore", "in the commit message"], answer: 1, why: ".gitignore holds the names and patterns to ignore." },
    ],
    [
      { tip: "Commit .gitignore. Tag releases with git tag v1.0." },
      {
        exercise: "ignore",
        task: 'In a new repository, make a .gitignore that ignores every .tmp file. Create keep.txt and cache.tmp, add everything with git add ., commit, tag the commit v1, and print the files in that commit with git ls-tree --name-only v1.',
        expected: [".gitignore", "keep.txt"],
        hints: ['echo "*.tmp" > .gitignore', "git add . respects .gitignore, so cache.tmp is left out."],
        solution: 'git init -q\necho "*.tmp" > .gitignore\necho "keep" > keep.txt\necho "junk" > cache.tmp\ngit add .\ngit commit -q -m "First"\ngit tag v1\ngit ls-tree --name-only v1',
      },
    ],
  ],
  recapPt: [
    ".gitignore lista nomes e padrões que o Git ignora; * é qualquer texto.",
    "Faz commit do próprio .gitignore.",
    "git tag v1.0 dá um nome permanente a um commit.",
  ],
  recap: [
    ".gitignore lists names and patterns Git ignores; * matches anything.",
    "Commit the .gitignore itself.",
    "git tag v1.0 gives a commit a permanent name.",
  ],
};
