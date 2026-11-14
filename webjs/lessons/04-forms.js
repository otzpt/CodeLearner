const { page } = require("./01-scripts.js");

module.exports = {
  id: "forms",
  title: "Reading what the visitor types",
  titlePt: "Ler o que o visitante escreve",
  screens: [
    [
      "An input's .value is what is typed in it right now. The \"input\" event fires on every change, so a page can react while the visitor types:",
      { code: page('<label for="name">Name</label>\n<input id="name" type="text">\n<p id="preview"></p>', 'const box = document.querySelector("#name");\nconst preview = document.querySelector("#preview");\nbox.addEventListener("input", () => {\n  preview.textContent = "Hello, " + box.value;\n});\nsim.type("#name", "Ana");   // PC only: type into the box\nconsole.log(preview.textContent);') },
      "sim.type plays the keyboard, PC-only like sim.click.",
    ],
    [
      ".value is always a string, even in a number box. Turn it into a number before doing maths, or + joins instead of adding:",
      { code: page('<input id="a" type="number">\n<input id="b" type="number">\n<button id="sum">Add</button>', 'document.querySelector("#sum").addEventListener("click", () => {\n  const a = document.querySelector("#a").value;\n  const b = document.querySelector("#b").value;\n  console.log(a + b);\n  console.log(Number(a) + Number(b));\n});\nsim.type("#a", "2");\nsim.type("#b", "3");\nsim.click("#sum");') },
      { quiz: "An input holds 10. What is input.value + 5?", choices: ["15", "\"105\""], answer: 2, why: ".value is a string. Use Number(input.value) + 5." },
    ],
    [
      "Never trust what was typed. Check it before using it, and tell the visitor what is wrong:",
      { code: page('<input id="age" type="number">\n<button id="go">Go</button>\n<p id="msg"></p>', 'const msg = document.querySelector("#msg");\ndocument.querySelector("#go").addEventListener("click", () => {\n  const age = Number(document.querySelector("#age").value);\n  if (Number.isNaN(age) || age < 0) {\n    msg.textContent = "Please type a real age.";\n  } else {\n    msg.textContent = "Thanks!";\n  }\n  console.log(msg.textContent);\n});\nsim.type("#age", "-4");\nsim.click("#go");\nsim.type("#age", "30");\nsim.click("#go");') },
      { tip: ".value is always text: convert with Number() and check it before use." },
    ],
    [
      {
        exercise: "forms",
        task: 'The page has an input "#celsius", a button "#convert" and a p "#out". On click, put the Fahrenheit value (C * 9 / 5 + 32) in #out followed by " F", and print it. Type 100 and click, then type 0 and click (with sim.type and sim.click, PC only).',
        expected: ["212 F", "32 F"],
        starter: page('<input id="celsius" type="number">\n<button id="convert">Convert</button>\n<p id="out"></p>', "// your code here"),
        hints: ['const c = Number(document.querySelector("#celsius").value);', 'out.textContent = (c * 9 / 5 + 32) + " F"; console.log(out.textContent);'],
        solution: page('<input id="celsius" type="number">\n<button id="convert">Convert</button>\n<p id="out"></p>', 'const out = document.querySelector("#out");\ndocument.querySelector("#convert").addEventListener("click", () => {\n  const c = Number(document.querySelector("#celsius").value);\n  out.textContent = (c * 9 / 5 + 32) + " F";\n  console.log(out.textContent);\n});\nsim.type("#celsius", "100");\nsim.click("#convert");\nsim.type("#celsius", "0");\nsim.click("#convert");'),
      },
    ],
  ],
  recapPt: [
    "input.value é o que está escrito agora; o evento \"input\" dispara a cada mudança.",
    ".value é sempre texto: converte com Number() antes de fazer contas.",
    "Verifica o que foi escrito antes de o usar.",
    "sim.type só existe no PC: faz de teclado.",
  ],
  recap: [
    "input.value is what is typed now; the \"input\" event fires on every change.",
    ".value is always text: convert with Number() before maths.",
    "Check what was typed before using it.",
    "sim.type only exists on the PC: it plays the keyboard.",
  ],
};
