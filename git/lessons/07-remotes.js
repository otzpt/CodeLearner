module.exports = {
  id: "remotes",
  title: "Sharing: clone, push and pull",
  titlePt: "Partilhar: clone, push e pull",
  screens: [
    [
      "A remote is another copy of the repository, usually on a server such as GitHub, that several people share. git clone copies one, with all its history. Here the \"server\" is just another folder, so the example runs without internet:",
      { code: 'git init -q --bare server.git\ngit clone -q server.git ana 2> /dev/null\ncd ana\necho "hello" > notes.txt\ngit add notes.txt\ngit commit -q -m "First note"\ngit push -q origin main\ngit log --format="%s" origin/main' },
      "--bare makes a repository with only history, the way servers keep them. origin is the name clone gives to where it copied from. git push sends your commits there.",
    ],
    [
      "git pull fetches what others pushed and merges it into your branch:",
      { code: 'git init -q --bare server.git\ngit clone -q server.git ana 2> /dev/null\ncd ana\necho "v1" > app.txt\ngit add app.txt\ngit commit -q -m "v1"\ngit push -q origin main\ncd ..\ngit clone -q server.git rui\ncd rui\necho "v2" > app.txt\ngit commit -q -am "v2 by Rui"\ngit push -q\ncd ../ana\ngit pull -q\ncat app.txt' },
      "Ana and Rui each have a full copy. Rui pushed, and Ana's pull brought his commit in.",
    ],
    [
      "If someone else pushed first, your push is refused until you pull their commits:",
      { code: 'git init -q --bare server.git\ngit clone -q server.git ana 2> /dev/null\ncd ana\necho "v1" > a.txt\ngit add a.txt\ngit commit -q -m "v1"\ngit push -q origin main\ncd ..\ngit clone -q server.git rui\ncd rui\necho "rui" > r.txt\ngit add r.txt\ngit commit -q -m "Rui"\ngit push -q\ncd ../ana\necho "ana" > n.txt\ngit add n.txt\ngit commit -q -m "Ana"\ngit push -q origin main', error: true },
      { quiz: "Your push is rejected because the remote has new commits. What first?", choices: ["push again", "git pull"], answer: 2, why: "Pull brings their commits in; then your push can go through." },
    ],
    [
      {
        exercise: "remotes",
        task: 'Make a bare repository hub.git and clone it into a folder me. In me, commit hello.txt containing "hi" and push. Then clone hub.git again into a folder check and print check/hello.txt.',
        expected: ["hi"],
        hints: ["git init -q --bare hub.git; git clone -q hub.git me 2> /dev/null; cd me", "After git push -q origin main: cd .., git clone -q hub.git check, cat check/hello.txt"],
        solution: 'git init -q --bare hub.git\ngit clone -q hub.git me 2> /dev/null\ncd me\necho "hi" > hello.txt\ngit add hello.txt\ngit commit -q -m "Hello"\ngit push -q origin main\ncd ..\ngit clone -q hub.git check\ncat check/hello.txt',
      },
    ],
  ],
  recapPt: [
    "Um remoto é outra cópia do repositório, normalmente num servidor.",
    "git clone copia um repositório e chama-lhe origin.",
    "git push envia os teus commits; git pull traz e junta os dos outros.",
    "Um push recusado quer dizer: faz pull primeiro.",
  ],
  recap: [
    "A remote is another copy of the repository, usually on a server.",
    "git clone copies a repository and names it origin.",
    "git push sends your commits; git pull fetches and merges others'.",
    "A rejected push means: pull first.",
  ],
};
