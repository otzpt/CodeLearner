module.exports = {
  id: "structure",
  title: "Page structure that means something",
  titlePt: "Estrutura com significado",
  screens: [
    [
      "A page usually has the same big parts: a header at the top, navigation links, the main content, and a footer. HTML has an element for each, so browsers, search engines and screen readers know which part is which:",
      { code: '<header>\n  <h1>Ana\'s blog</h1>\n  <nav>\n    <a href="index.html">Home</a>\n    <a href="about.html">About</a>\n  </nav>\n</header>\n<main>\n  <article>\n    <h2>First post</h2>\n    <p>Hello!</p>\n  </article>\n</main>\n<footer>\n  <p>Written by Ana</p>\n</footer>' },
    ],
    [
      "These draw exactly like div. The difference is meaning: a screen reader can jump straight to main, skipping the menu; a search engine knows the article is the content. Use div only when no other element fits.",
      "Headings should go in order: one h1 for the page, h2 for its sections, h3 inside those. Skipping from h1 to h4 because it looks smaller breaks that outline; size is CSS's job.",
      { quiz: "Why use <nav> instead of <div> for the menu?", choices: ["it draws differently", "it tells browsers and screen readers what it is"], answer: 2, why: "They look the same; nav carries meaning." },
    ],
    [
      {
        exercise: "structure",
        task: 'Make a header holding an h1 "Shop", a main holding an h2 "Offers" and a paragraph "Nothing yet.", and a footer holding a paragraph "2026".',
        expected: ["header", "  h1: Shop", "main", "  h2: Offers", "  p: Nothing yet.", "footer", "  p: 2026"],
        starter: "<!-- write your HTML below -->",
        hints: ["Three elements side by side: header, main, footer.", "Each holds its heading or paragraph inside."],
        solution: "<header>\n  <h1>Shop</h1>\n</header>\n<main>\n  <h2>Offers</h2>\n  <p>Nothing yet.</p>\n</main>\n<footer>\n  <p>2026</p>\n</footer>",
      },
    ],
  ],
  recapPt: [
    "header, nav, main, article e footer dizem o que cada parte é.",
    "Desenham como div, mas ajudam leitores de ecrã e motores de pesquisa.",
    "Os títulos vão por ordem: um h1, depois h2, depois h3; o tamanho é do CSS.",
  ],
  recap: [
    "header, nav, main, article and footer say what each part is.",
    "They draw like div, but help screen readers and search engines.",
    "Headings go in order: one h1, then h2, then h3; size is CSS's job.",
  ],
};
