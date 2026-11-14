module.exports = {
  id: "lists-and-tables",
  title: "Lists and tables",
  titlePt: "Listas e tabelas",
  screens: [
    [
      "ul is a list with bullets, ol a numbered list. Each item is an li inside it:",
      { code: "<ul>\n  <li>milk</li>\n  <li>bread</li>\n</ul>\n<ol>\n  <li>Mix</li>\n  <li>Bake</li>\n</ol>" },
      "The browser draws the bullets and numbers itself. A list can hold another list inside an li.",
    ],
    [
      "A table holds data in rows and columns. tr is a row, th a header cell, td a normal cell:",
      { code: "<table>\n  <tr><th>Name</th><th>Grade</th></tr>\n  <tr><td>Ana</td><td>15</td></tr>\n  <tr><td>Rui</td><td>12</td></tr>\n</table>" },
      "Use tables for data that really is a grid, not to arrange a page's layout: that is CSS's job.",
      { quiz: "Which element is one item of a list?", choices: ["ul", "li", "ol"], answer: 2, why: "ul and ol are the whole list; li is each item." },
    ],
    [
      {
        exercise: "lists",
        task: 'Make a heading h2 "Steps" and a numbered list with "Open", "Read" and "Close".',
        expected: ["h2: Steps", "ol", "  li: Open", "  li: Read", "  li: Close"],
        starter: "<!-- write your HTML below -->",
        hints: ["A numbered list is ol.", "<ol><li>Open</li> ... </ol>"],
        solution: "<h2>Steps</h2>\n<ol>\n  <li>Open</li>\n  <li>Read</li>\n  <li>Close</li>\n</ol>",
      },
    ],
  ],
  recapPt: [
    "ul é uma lista com marcas, ol uma lista numerada; cada item é um li.",
    "Uma tabela tem linhas tr com células th (cabeçalho) ou td.",
    "Tabelas são para dados em grelha, não para arrumar a página.",
  ],
  recap: [
    "ul is a bulleted list, ol a numbered one; each item is an li.",
    "A table has tr rows with th (header) or td cells.",
    "Tables are for grid data, not for page layout.",
  ],
};
