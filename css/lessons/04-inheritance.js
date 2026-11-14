const { page } = require("./01-rules.js");

module.exports = {
  id: "inheritance",
  title: "Inheritance",
  titlePt: "Herança",
  screens: [
    [
      "Some properties pass down from an element to everything inside it: text properties like color, font-size, font-family and text-align. Set them once on body and the whole page gets them:",
      { code: page("body {\n  color: #333;\n  font-family: sans-serif;\n}", "<body>\n  <h1>Title</h1>\n  <p>Text with <em>a word</em></p>\n</body>") },
      "\"inherited from body\" means the element has no rule of its own for it, so it takes its parent's value.",
    ],
    [
      "A rule that matches the element itself always beats an inherited value, however specific the parent's rule was:",
      { code: page("#page {\n  color: gray;\n}\np {\n  color: black;\n}", '<body id="page">\n  <p>Black, from its own rule</p>\n  <h2>Gray, inherited</h2>\n</body>') },
      "Box properties, such as border, margin, padding and background, do not pass down: a border on a div does not draw borders around everything inside it.",
      { quiz: "body has color: navy. A p inside has no color rule. What colour is it?", choices: ["the browser default", "navy, inherited"], answer: 2, why: "color passes down from parent to child." },
    ],
    [
      "Links are an exception the browser makes: they keep their own link colour instead of inheriting it. To change it, select a directly.",
      { tip: "Text properties pass down; box properties do not. A rule on the element itself beats anything inherited." },
      {
        exercise: "inheritance",
        task: 'Set font-size: 20px and color: darkgreen on the body, and color: black on .plain. The body holds a p "Green" and a p with class plain "Black".',
        expected: ["p", "  color: darkgreen   (inherited from body)", "  font-size: 20px   (inherited from body)", "p.plain", "  color: black   (from .plain)", "  font-size: 20px   (inherited from body)"],
        starter: page("  /* your CSS here */", '<body>\n  <p>Green</p>\n  <p class="plain">Black</p>\n</body>'),
        hints: ["body { font-size: 20px; color: darkgreen; }", ".plain { color: black; }"],
        solution: page("body { font-size: 20px; color: darkgreen; }\n.plain { color: black; }", '<body>\n  <p>Green</p>\n  <p class="plain">Black</p>\n</body>'),
      },
    ],
  ],
  recapPt: [
    "color, font-size, font-family e text-align passam de pai para filho.",
    "Uma regra do próprio elemento ganha sempre ao valor herdado.",
    "border, margin, padding e background não passam.",
    "As ligações mantêm a sua cor; muda-as escolhendo a.",
  ],
  recap: [
    "color, font-size, font-family and text-align pass from parent to child.",
    "A rule on the element itself always beats an inherited value.",
    "border, margin, padding and background do not pass down.",
    "Links keep their own colour; change it by selecting a.",
  ],
};
