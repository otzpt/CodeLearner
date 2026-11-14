// The ask() helper every program in this lesson starts with. One copy, so the
// lesson, the examples, the starter file and lesson 13 cannot disagree.
const HELPER = `const readline = require("node:readline");
const rl = readline.createInterface({ input: process.stdin });
const lines = rl[Symbol.asyncIterator]();

async function ask(question) {
  process.stdout.write(question);
  const answer = (await lines.next()).value ?? "";
  if (!process.stdin.isTTY) console.log(answer); // show answers typed by the checker
  return answer;
}`;

module.exports = {
  HELPER,
  id: "input",
  title: "Asking the user",
  screens: [
    [
      "Node has no single \"ask and wait\" instruction. These lines make one, called ask. Copy them to the top of any program that asks questions:",
      { code: HELPER, run: false },
      "You do not need to understand them yet. The exercise file already has them.",
    ],
    [
      "Code that waits for an answer goes in an async function, with await before ask. rl.close() at the end lets the program finish:",
      { setup: HELPER, code: 'async function main() {\n  const name = await ask("Name? ");\n  console.log(`Hi, ${name}!`);\n  rl.close();\n}\nmain();', input: ["Ana"] },
      "Forget await and you get [object Promise] instead of the answer.",
    ],
    [
      "ask always gives a string, even for numbers:",
      { setup: HELPER, code: 'async function main() {\n  const text = await ask("Number? ");\n  console.log(text + 1);\n  console.log(Number(text) + 1);\n  rl.close();\n}\nmain();', input: ["5"] },
      { quiz: "The user types 10. What does answer + 5 give?", choices: ["15", "105"], answer: 2, why: "answer is the string \"10\". Use Number(answer) + 5." },
    ],
    [
      {
        exercise: "ask",
        task: 'Ask "Your name? " and "Your age? ". Print: Hi NAME, next year you will be AGE+1.',
        input: ["Rita", "20"],
        expected: ["Your name? Rita", "Your age? 20", "Hi Rita, next year you will be 21."],
        starter: `${HELPER}\n\nasync function main() {\n  // your code here\n\n  rl.close();\n}\n\nmain();`,
        hints: ['const name = await ask("Your name? ");', "Number(age) + 1 inside the template string."],
        solution: `${HELPER}\n\nasync function main() {\n  const name = await ask("Your name? ");\n  const age = await ask("Your age? ");\n  console.log(\`Hi \${name}, next year you will be \${Number(age) + 1}.\`);\n  rl.close();\n}\n\nmain();`,
      },
    ],
  ],
  recap: [
    "Put the ask helper at the top; use it inside an async function with await.",
    "End with rl.close().",
    "ask gives a string; Number(text) makes it a number.",
  ],
};
