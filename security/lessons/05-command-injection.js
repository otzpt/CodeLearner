module.exports = {
  id: "command-injection",
  title: "Command injection",
  titlePt: "Injeção de comandos",
  screens: [
    [
      "Programs often run other programs. subprocess.run does it. The mistake is the same as with SQL: building one command line out of text the user typed and handing it to the shell:",
      { code: 'import subprocess\n\ndef greet(name):\n    subprocess.run(f"echo Hello {name}", shell=True)\n\ngreet("Ana")\ngreet("Ana; echo HACKED")' },
      "The shell reads ; as \"and then run another command\", so the user's text ran a command of their choosing. With rm instead of echo, files would be gone.",
    ],
    [
      "The fix: pass a list, the program first and then each argument, without shell=True. No shell reads the text, so ; is just a character:",
      { code: 'import subprocess\n\ndef greet(name):\n    subprocess.run(["echo", "Hello", name])\n\ngreet("Ana")\ngreet("Ana; echo HACKED")' },
      { quiz: "Which call is safe with text from a user?", choices: ['run(f"ls {path}", shell=True)', 'run(["ls", path])'], answer: 2, why: "A list passes the text as one argument; no shell interprets it." },
    ],
    [
      {
        exercise: "commands",
        task: 'Write count_words(text) that runs ["wc", "-w"] with text passed as its input (input=text, capture_output=True, text=True) and returns the number as an int. Print count_words("one two three") and count_words("a; echo hacked").',
        expected: ["3", "3"],
        hints: ['result = subprocess.run(["wc", "-w"], input=text, capture_output=True, text=True)', "int(result.stdout) turns the output into a number."],
        solution: 'import subprocess\n\ndef count_words(text):\n    result = subprocess.run(["wc", "-w"], input=text, capture_output=True, text=True)\n    return int(result.stdout)\n\nprint(count_words("one two three"))\nprint(count_words("a; echo hacked"))',
      },
    ],
  ],
  recapPt: [
    "shell=True com texto do utilizador deixa-o correr os comandos que quiser.",
    "Passa uma lista: subprocess.run([\"programa\", arg1, arg2]).",
    "Assim nenhuma shell interpreta ; | & e afins.",
  ],
  recap: [
    "shell=True with user text lets them run any command they like.",
    "Pass a list: subprocess.run([\"program\", arg1, arg2]).",
    "Then no shell interprets ; | & and the like.",
  ],
};
