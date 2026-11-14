const { page } = require("./01-scripts.js");

module.exports = {
  id: "events",
  title: "Reacting to clicks: events",
  titlePt: "Reagir a cliques: eventos",
  screens: [
    [
      "A script usually runs once, when the page loads. To react later, it gives the browser a function to call when something happens: an event. addEventListener(\"click\", function) runs the function on every click:",
      { code: page('<button id="hello">Say hello</button>', 'const button = document.querySelector("#hello");\nbutton.addEventListener("click", () => {\n  console.log("Hello!");\n});\nconsole.log("listener added");\nsim.click("#hello");   // PC only: click it\nsim.click("#hello");') },
      "sim.click plays your mouse. It is not part of the browser; anything starting with sim. is PC-only.",
    ],
    [
      "Notice the order: \"listener added\" printed first. The function does not run when it is added, only when the click comes. Everything a page does after loading works this way.",
      "A counter keeps a variable outside the function, so it survives between clicks:",
      { code: page('<button id="add">+1</button>\n<p id="count">0</p>', 'let count = 0;\nconst shown = document.querySelector("#count");\ndocument.querySelector("#add").addEventListener("click", () => {\n  count = count + 1;\n  shown.textContent = count;\n});\nsim.click("#add");\nsim.click("#add");\nsim.click("#add");\nconsole.log(shown.textContent);') },
      { quiz: "When does the function given to addEventListener run?", choices: ["right away", "each time the event happens"], answer: 2, why: "It is called by the browser, on every click." },
    ],
    [
      "A click also travels up through every element that contains the one clicked. The function gets an event object; event.target is the element actually clicked:",
      { code: page('<ul id="list">\n  <li id="a">Apples</li>\n  <li id="b">Bread</li>\n</ul>', 'document.querySelector("#list").addEventListener("click", (event) => {\n  console.log("clicked", event.target.textContent);\n});\nsim.click("#b");\nsim.click("#a");') },
      "One listener on the list handles clicks on every item, even items added later.",
      { tip: "addEventListener(\"click\", fn) runs fn on each click; event.target is what was clicked." },
    ],
    [
      {
        exercise: "events",
        task: 'The page has a button "#toggle" and a p "#lamp" with text "off". Each click switches the text between "off" and "on". Click it three times with sim.click (PC only), printing the text after each click.',
        expected: ["on", "off", "on"],
        starter: page('<button id="toggle">Switch</button>\n<p id="lamp">off</p>', "// your code here"),
        hints: ['lamp.textContent = lamp.textContent === "off" ? "on" : "off";', "Inside the listener, print lamp.textContent after changing it."],
        solution: page('<button id="toggle">Switch</button>\n<p id="lamp">off</p>', 'const lamp = document.querySelector("#lamp");\ndocument.querySelector("#toggle").addEventListener("click", () => {\n  lamp.textContent = lamp.textContent === "off" ? "on" : "off";\n  console.log(lamp.textContent);\n});\nsim.click("#toggle");\nsim.click("#toggle");\nsim.click("#toggle");'),
      },
    ],
  ],
  recapPt: [
    "addEventListener(\"click\", função) chama a função em cada clique.",
    "A função não corre quando é adicionada, só quando o evento acontece.",
    "O clique sobe pelos elementos que contêm o clicado; event.target é o clicado.",
    "sim.click só existe no PC: faz de rato.",
  ],
  recap: [
    "addEventListener(\"click\", function) calls the function on each click.",
    "The function does not run when added, only when the event happens.",
    "A click travels up through the containing elements; event.target is the one clicked.",
    "sim.click only exists on the PC: it plays the mouse.",
  ],
};
