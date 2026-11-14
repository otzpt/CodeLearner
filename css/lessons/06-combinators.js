const { page } = require("./01-rules.js");

module.exports = {
  id: "combinators",
  title: "Elements inside elements",
  titlePt: "Elementos dentro de elementos",
  screens: [
    [
      "A space between two selectors means \"inside\": nav a picks a elements anywhere inside a nav, however deep:",
      { code: page("nav a {\n  font-weight: bold;\n}", '<nav>\n  <ul><li><a href="/">Home</a></li></ul>\n</nav>\n<p><a href="/help">Help</a></p>') },
      "The second link is not inside a nav, so it is not bold.",
    ],
    [
      "> means \"directly inside\": a child, not a grandchild:",
      { code: page(".menu > li {\n  color: navy;\n}", '<ul class="menu">\n  <li>Top\n    <ul><li>Nested</li></ul>\n  </li>\n</ul>') },
      "Only the top li is a direct child of .menu. The nested li sits inside another ul, so it is not navy by this rule; it still inherits navy from its parent li, because color passes down.",
      { quiz: "Which picks only direct children?", choices: ["article p", "article > p"], answer: 2, why: "> is direct children; a space is any depth." },
    ],
    [
      {
        exercise: "combinators",
        task: 'Make p inside .card have color: gray, but a p directly inside .card > footer have font-size: 12px. The page: a div.card holding a p "Body" and a footer holding a p "Small print".',
        expected: ["div.card", "  p", "    color: gray   (from .card p)", "  footer", "    p", "      color: gray   (from .card p)", "      font-size: 12px   (from .card > footer > p)"],
        starter: page("  /* your CSS here */", '<div class="card">\n  <p>Body</p>\n  <footer>\n    <p>Small print</p>\n  </footer>\n</div>'),
        hints: [".card p { color: gray; }", ".card > footer > p { font-size: 12px; }"],
        solution: page(".card p { color: gray; }\n.card > footer > p { font-size: 12px; }", '<div class="card">\n  <p>Body</p>\n  <footer>\n    <p>Small print</p>\n  </footer>\n</div>'),
      },
    ],
  ],
  recapPt: [
    "\"a b\" escolhe b dentro de a, a qualquer profundidade.",
    "\"a > b\" escolhe só b diretamente dentro de a.",
    "Combinações contam para a especificidade como a soma das partes.",
  ],
  recap: [
    "\"a b\" picks b inside a, at any depth.",
    "\"a > b\" picks only b directly inside a.",
    "Combined selectors count towards specificity as the sum of their parts.",
  ],
};
