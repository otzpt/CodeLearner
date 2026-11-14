const { page } = require("./01-rules.js");

module.exports = {
  id: "important-and-inline",
  title: "style attributes and !important",
  titlePt: "Atributos style e !important",
  screens: [
    [
      "CSS can also go straight on an element, in its style attribute. It beats every rule in the style sheet:",
      { code: page("#title {\n  color: blue;\n}", '<h1 id="title" style="color: red">Red</h1>') },
      "It works, but scatters the look of a page across the HTML. Keep it for quick tests.",
    ],
    [
      "!important after a value makes a declaration beat every normal one, whatever the specificity:",
      { code: page("p {\n  color: green !important;\n}\n#intro {\n  color: red;\n}", '<p id="intro" style="color: blue">Still green</p>') },
      "It is tempting when a style \"will not apply\", but the next rule then needs !important to win back, and the style sheet turns into a fight. Fix the specificity instead.",
      { quiz: "What is usually better than adding !important?", choices: ["another !important", "a more specific selector, or fewer competing rules"], answer: 2, why: "!important only escalates the fight." },
    ],
    [
      {
        exercise: "important",
        task: 'A p has id "msg" and style="color: blue". Add a rule #msg { color: orange; } and a rule p { color: purple !important; }. Which colour wins?',
        expected: ["p#msg", "  color: purple   (from p)"],
        starter: page("  /* your CSS here */", '<p id="msg" style="color: blue">Hello</p>'),
        hints: ["Write both rules exactly as given.", "!important beats both the id rule and the style attribute."],
        solution: page("#msg { color: orange; }\np { color: purple !important; }", '<p id="msg" style="color: blue">Hello</p>'),
      },
    ],
  ],
  recapPt: [
    "O atributo style ganha a todas as regras normais da folha de estilos.",
    "!important ganha a tudo o que é normal, seja qual for a especificidade.",
    "Evita os dois: corrige a especificidade em vez disso.",
  ],
  recap: [
    "The style attribute beats every normal rule in the style sheet.",
    "!important beats everything normal, whatever the specificity.",
    "Avoid both: fix the specificity instead.",
  ],
};
