module.exports = {
  id: "tags",
  title: "Tags and elements",
  titlePt: "Tags e elementos",
  screens: [
    [
      "A web page is a text file written in HTML. HTML is not a programming language: it describes what each part of the page is. A heading, a paragraph, a link. The browser reads it and draws the page.",
      "A terminal cannot draw a page, so this course shows what the browser builds from your HTML: a tree of elements, one per line, indented inside its parent. That tree is what the browser draws from.",
    ],
    [
      "A tag is a word in angle brackets. Most come in pairs: <p> opens a paragraph and </p>, with a slash, closes it. Together with the text between them they make an element:",
      { code: "<h1>My first page</h1>\n<p>Hello! This is a paragraph.</p>" },
      "h1 is the main heading; h2 to h6 are smaller headings under it. p is a paragraph.",
    ],
    [
      "Elements go inside other elements. Here <strong> marks important words inside a paragraph, and the tree shows it indented under the p:",
      { code: "<p>This is <strong>very</strong> important.</p>" },
      "The browser draws strong text in bold. em marks emphasis, usually drawn in italics.",
      { quiz: "Which tag closes a paragraph?", choices: ["<p>", "</p>", "<p/>"], answer: 2, why: "A closing tag has a slash before the name." },
    ],
    [
      { tip: "An element is an opening tag, its content, and a closing tag with a slash." },
      {
        exercise: "tags",
        task: 'Write a page with a main heading "Recipes", a smaller heading (h2) "Pancakes", and a paragraph "Mix and fry." in which the word "fry" is marked important.',
        expected: ["h1: Recipes", "h2: Pancakes", "p: Mix and fry.", "  strong: fry"],
        starter: "<!-- write your HTML below -->",
        hints: ["<h1>Recipes</h1> then <h2>Pancakes</h2>", "<p>Mix and <strong>fry</strong>.</p>"],
        solution: "<h1>Recipes</h1>\n<h2>Pancakes</h2>\n<p>Mix and <strong>fry</strong>.</p>",
      },
    ],
  ],
  recapPt: [
    "HTML descreve o que cada parte da página é; o navegador desenha-a.",
    "<p> abre e </p> fecha; juntos, com o conteúdo, são um elemento.",
    "h1 a h6 são títulos, p é um parágrafo, strong e em marcam palavras.",
    "Os elementos ficam dentro uns dos outros, formando uma árvore.",
  ],
  recap: [
    "HTML describes what each part of a page is; the browser draws it.",
    "<p> opens and </p> closes; together with the content they are an element.",
    "h1 to h6 are headings, p is a paragraph, strong and em mark words.",
    "Elements sit inside each other, forming a tree.",
  ],
};
