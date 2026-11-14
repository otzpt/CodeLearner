const page = (css, body) => `<style>\n${css}\n</style>\n${body}`;

module.exports = {
  page,
  id: "rules",
  title: "Rules: selector and properties",
  titlePt: "Regras: seletor e propriedades",
  screens: [
    [
      "HTML says what each part of a page is; CSS says how it looks: colours, sizes, spacing, layout. CSS can go in a <style> element in the page's head.",
      "A terminal cannot draw the page, so this course shows the style each element ends up with, and which rule gave it. That is exactly the part of CSS that surprises people.",
    ],
    [
      "A rule is a selector, which picks elements, and declarations in { }, each a property, a colon, a value and a semicolon:",
      { code: page("p {\n  color: navy;\n  font-size: 18px;\n}", "<h1>Title</h1>\n<p>First</p>\n<p>Second</p>") },
      "p as a selector picks every p element. The h1 is not a p, so it gets nothing.",
    ],
    [
      "Several selectors can share one rule, separated by commas. A mistake in a declaration, such as a missing colon, makes the browser skip that declaration silently:",
      { code: page("h1, p {\n  color: green;\n}\np {\n  font-size 30px;\n  font-weight: bold;\n}", "<h1>Title</h1>\n<p>Text</p>") },
      "font-size 30px has no colon, so it is ignored, without any error message. If a style does not apply, check the punctuation first.",
      { quiz: "In p { color: red; }, which part is the selector?", choices: ["p", "color", "red"], answer: 1, why: "The selector picks elements; color: red is a declaration." },
    ],
    [
      {
        exercise: "rules",
        task: 'In the <style>, make every h2 have color: purple, and every li have font-size: 14px. The body holds <h2>Menu</h2> and a ul with two li: "Soup" and "Salad".',
        expected: ["h2", "  color: purple   (from h2)", "ul", "  li", "    font-size: 14px   (from li)", "  li", "    font-size: 14px   (from li)"],
        starter: page("  /* your CSS here */", "<h2>Menu</h2>\n<ul>\n  <li>Soup</li>\n  <li>Salad</li>\n</ul>"),
        hints: ["h2 { color: purple; }", "li { font-size: 14px; }"],
        solution: page("h2 { color: purple; }\nli { font-size: 14px; }", "<h2>Menu</h2>\n<ul>\n  <li>Soup</li>\n  <li>Salad</li>\n</ul>"),
      },
    ],
  ],
  recapPt: [
    "CSS diz como a página fica; pode ir num <style>.",
    "Uma regra é um seletor e declarações: p { color: navy; }.",
    "Vírgulas juntam seletores; uma declaração mal escrita é ignorada sem aviso.",
  ],
  recap: [
    "CSS says how the page looks; it can go in a <style>.",
    "A rule is a selector and declarations: p { color: navy; }.",
    "Commas share a rule; a mistyped declaration is silently ignored.",
  ],
};
