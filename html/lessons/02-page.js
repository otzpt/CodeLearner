module.exports = {
  id: "page",
  title: "A whole page",
  titlePt: "Uma página inteira",
  screens: [
    [
      "A complete page has a fixed frame. <!DOCTYPE html> says it is modern HTML. Everything sits in <html>, which holds two parts: <head>, information about the page, and <body>, what is drawn:",
      { code: '<!DOCTYPE html>\n<html lang="en">\n<head>\n  <meta charset="utf-8">\n  <title>My page</title>\n</head>\n<body>\n  <h1>Welcome</h1>\n</body>\n</html>' },
      "The title is the text on the browser tab, not on the page. meta charset=\"utf-8\" lets the page use any language's letters. lang says which language the text is in.",
    ],
    [
      "Some elements are empty and have no closing tag: meta, img, br (a line break), hr (a dividing line), input. Writing a closing tag for one is a mistake the browser ignores:",
      { code: "<p>First line<br>second line</p>\n<hr>\n<p>After the line</p>" },
      "Spaces and line breaks in the HTML file do not matter: the browser joins all spacing into single spaces. Use p, br and headings for layout, not empty lines.",
      { quiz: "Where does the text of <title> appear?", choices: ["at the top of the page", "on the browser tab"], answer: 2, why: "The title is information about the page, in head." },
    ],
    [
      {
        exercise: "page",
        task: 'Write a complete page with the language "en", a title "Notes", and in the body a heading "Today" and two paragraphs: "Buy milk" and "Call Rui".',
        expected: ['html [lang="en"]', "  head", "    title: Notes", "  body", "    h1: Today", "    p: Buy milk", "    p: Call Rui"],
        hints: ['<!DOCTYPE html>, then <html lang="en">, <head> with <title>, then <body>.', "Close body and html at the end."],
        solution: '<!DOCTYPE html>\n<html lang="en">\n<head>\n  <title>Notes</title>\n</head>\n<body>\n  <h1>Today</h1>\n  <p>Buy milk</p>\n  <p>Call Rui</p>\n</body>\n</html>',
      },
    ],
  ],
  recapPt: [
    "<!DOCTYPE html>, depois <html> com <head> (informação) e <body> (o que se vê).",
    "<title> é o texto do separador do navegador.",
    "meta, img, br, hr e input não têm etiqueta de fecho.",
    "Espaços e mudanças de linha no ficheiro não contam para o desenho.",
  ],
  recap: [
    "<!DOCTYPE html>, then <html> holding <head> (information) and <body> (what is drawn).",
    "<title> is the text on the browser tab.",
    "meta, img, br, hr and input have no closing tag.",
    "Spacing and line breaks in the file do not affect the drawing.",
  ],
};
