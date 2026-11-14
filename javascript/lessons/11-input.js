// The ask() helper every program in this lesson starts with. One copy, so the
// lesson text, the examples and the starter file cannot disagree.
const HELPER = `const readline = require("node:readline");
const rl = readline.createInterface({ input: process.stdin });
const lines = rl[Symbol.asyncIterator]();

async function ask(question) {
  process.stdout.write(question);
  return (await lines.next()).value ?? "";
}`;

module.exports = {
  HELPER,
  id: "input",
  title: "Asking the user",
  titlePt: "Perguntar ao utilizador",
  screens: [
    [
      "So far every program printed the same thing every time. To react to the person using it, a program has to read what they type.",
      "Node has no single instruction for \"ask a question and wait\". These lines build one, called ask. Copy them to the top of any program that asks questions; you do not need to understand every line yet:",
      { code: HELPER, run: false },
    ],
    [
      "ask has to wait for you to type, and waiting needs two special words. The function that uses ask is marked async, and each call is marked await. At the end, rl.close() lets the program finish:",
      { setup: HELPER, code: 'async function main() {\n  const name = await ask("What is your name? ");\n  console.log(`Hello, ${name}!`);\n  rl.close();\n}\n\nmain();', output: ["What is your name? {Ana}", "Hello, Ana!"] },
      "Forget the await and name does not hold the answer yet, so the greeting comes out wrong. If a program prints [object Promise] or Promise { <pending> }, look for a missing await.",
    ],
    [
      "What ask gives back is always a string, even when the person types a number. Remember from lesson 2 what + does with a string:",
      { setup: HELPER, code: 'async function main() {\n  const text = await ask("A number: ");\n  console.log(text + 1);\n  console.log(Number(text) + 1);\n  rl.close();\n}\n\nmain();', output: ["A number: {5}", "51", "6"] },
      "Number(...) turns the typed text into a number. If the text is not a number, Number(\"hello\") for example, you get NaN, short for \"not a number\". Number.isNaN(value) checks for it.",
      { quiz: "A user types 10. What does console.log(answer + 5) print?", choices: ["15", "105"], answer: 2, why: "answer is the string \"10\", so + joins. Use Number(answer) + 5." },
    ],
    [
      {
        exercise: "ask",
        task: "Ask \"Your name? \" and then \"Your age? \". Print: Hi NAME, next year you will be AGE+1.",
        expected: ["Your name? {Rita}", "Your age? {20}", "Hi Rita, next year you will be 21."],
        starter: `${HELPER}\n\nasync function main() {\n  // your code here\n\n  rl.close();\n}\n\nmain();`,
        hints: ["const name = await ask(\"Your name? \"); and the same for age.", "Number(age) + 1 inside the template string."],
        solution: `${HELPER}\n\nasync function main() {\n  const name = await ask("Your name? ");\n  const age = await ask("Your age? ");\n  console.log(\`Hi \${name}, next year you will be \${Number(age) + 1}.\`);\n  rl.close();\n}\n\nmain();`,
      },
    ],
  ],
  recapPt: [
    "Ler o que o utilizador escreve precisa da função ask no topo do ficheiro.",
    "O código que usa ask vai numa função async, com await antes de cada ask, e rl.close() no fim.",
    "ask devolve sempre uma string. Number(texto) transforma-a em número; NaN quer dizer que não era um.",
  ],
  recap: [
    "Reading what the user types needs the ask helper at the top of the file.",
    "Code that uses ask goes in an async function, with await before each ask, and rl.close() at the end.",
    "ask always gives a string. Number(text) turns it into a number; NaN means it was not one.",
  ],
};
