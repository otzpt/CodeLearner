module.exports = {
  id: "nesting",
  title: "Nesting and closing tags",
  titlePt: "Encaixar e fechar tags",
  screens: [
    [
      "Elements must close in the reverse order they opened, like boxes inside boxes: the last one opened is the first one closed. Here div, a box for grouping, holds a heading and a paragraph:",
      { code: "<div>\n  <h2>Box</h2>\n  <p>Inside the <em>box</em>.</p>\n</div>" },
      "div draws nothing by itself. It groups things, so CSS can style them together later.",
    ],
    [
      "Close them in the wrong order, or forget one, and the browser has to guess. It does not stop or show an error: it quietly builds a different tree, which then looks or behaves wrong. This course shows a warning instead:",
      { code: "<div>\n  <p>Open paragraph\n</div>\n<p>Next</p>", warning: true },
      "The p was still open when the div closed, so the browser closed it there. Always close what you open.",
      { quiz: "Which is nested correctly?", choices: ["<p><em>hi</p></em>", "<p><em>hi</em></p>"], answer: 2, why: "em opened last, so it closes first." },
    ],
    [
      "span is the inline twin of div: it groups a few words inside a line without breaking it:",
      { code: '<p>Price: <span class="price">5 euros</span></p>' },
      "class gives an element a name for styling, used in the CSS course. id is the same but must be unique on the page.",
      { tip: "Close in reverse order of opening. div groups blocks; span groups words." },
    ],
    [
      {
        exercise: "nesting",
        task: 'Make a div with the id "card" holding a heading h2 "Ana" and a paragraph "Likes ", followed by the word "cats" in em, then ".".',
        expected: ["div#card", "  h2: Ana", "  p: Likes cats.", "    em: cats"],
        starter: "<!-- write your HTML below -->",
        hints: ['<div id="card"> ... </div>', "<p>Likes <em>cats</em>.</p> inside the div, after the h2."],
        solution: '<div id="card">\n  <h2>Ana</h2>\n  <p>Likes <em>cats</em>.</p>\n</div>',
      },
    ],
  ],
  recapPt: [
    "Fecha os elementos pela ordem inversa à que os abriste.",
    "Um erro nas tags não dá erro: o navegador adivinha e constrói outra árvore.",
    "div agrupa blocos; span agrupa palavras dentro de uma linha.",
    "class dá um nome para estilos; id é um nome único.",
  ],
  recap: [
    "Close elements in the reverse order you opened them.",
    "A tag mistake gives no error: the browser guesses and builds a different tree.",
    "div groups blocks; span groups words within a line.",
    "class gives a name for styling; id is a unique name.",
  ],
};
