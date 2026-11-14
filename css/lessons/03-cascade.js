const { page } = require("./01-rules.js");

module.exports = {
  id: "cascade",
  title: "When rules disagree: the cascade",
  titlePt: "Quando as regras discordam: a cascata",
  screens: [
    [
      "Often two rules set the same property on the same element. The browser has to pick one. If the selectors are equally specific, the rule written last wins:",
      { code: page("p {\n  color: blue;\n}\np {\n  color: green;\n}", "<p>Which colour?</p>") },
      "This is why it is called cascading: later rules flow over earlier ones.",
    ],
    [
      "But a more specific selector wins wherever it is written. An id beats any number of classes, and a class beats any number of element names:",
      { code: page("#main {\n  color: red;\n}\n.text {\n  color: orange;\n}\np {\n  color: blue;\n}", '<p id="main" class="text">Red wins</p>\n<p class="text">Orange wins</p>') },
      "p comes last, yet loses both times: #main and .text are more specific than p.",
    ],
    [
      "Count specificity as three numbers: ids, classes, element names. #nav a is (1, 0, 1); .menu .item is (0, 2, 0). Compare from the left: the id wins.",
      { code: page(".menu .item {\n  color: green;\n}\n#nav a {\n  color: purple;\n}", '<ul id="nav" class="menu">\n  <li class="item"><a class="item" href="/">Home</a></li>\n</ul>') },
      "When a style \"does not work\", a more specific rule elsewhere is usually winning. This course shows which rule won; in a browser, the developer tools (F12) do the same.",
      { quiz: "Which wins: .card p or #intro?", choices: [".card p", "#intro"], answer: 2, why: "An id (1,0,0) beats one class and one element (0,1,1)." },
    ],
    [
      {
        exercise: "cascade",
        task: 'The page has a p with id "promo" and class "note". Write three rules: p sets color: black, .note sets color: blue, #promo sets color: gold, in that order. Then a second rule for p, after them, that sets color: red. Which colour does the p end with?',
        expected: ["p#promo.note", "  color: gold   (from #promo)"],
        starter: page("  /* your CSS here */", '<p id="promo" class="note">Sale!</p>'),
        hints: ["Write all four rules; the order matters only between equally specific selectors.", "#promo is the most specific, so it wins even though p comes last."],
        solution: page("p { color: black; }\n.note { color: blue; }\n#promo { color: gold; }\np { color: red; }", '<p id="promo" class="note">Sale!</p>'),
      },
    ],
  ],
  recapPt: [
    "Com a mesma especificidade, ganha a regra escrita por último.",
    "Uma regra mais específica ganha onde quer que esteja: id > classe > elemento.",
    "Conta (ids, classes, elementos) e compara da esquerda.",
  ],
  recap: [
    "With equal specificity, the rule written last wins.",
    "A more specific rule wins wherever it is: id > class > element.",
    "Count (ids, classes, elements) and compare from the left.",
  ],
};
