// A page with a script at the end of its body.
const page = (body, script) => `${body}\n<script>\n${script}\n</script>`;

module.exports = {
  page,
  id: "scripts",
  title: "A script on a page",
  titlePt: "Um script numa página",
  screens: [
    [
      "HTML says what is on a page, CSS how it looks, and JavaScript what it does: react to clicks, check forms, change the page while it is open. The JavaScript goes in a <script> element, usually at the end of body, so the page above it already exists when it runs.",
      "A terminal cannot show a page, so this course runs your page's scripts against a stand-in for the browser and shows what they print with console.log. In a real browser, console.log output is in the developer tools (F12, the Console tab).",
    ],
    [
      "document is the page, as an object the script can use. document.querySelector takes a CSS selector and gives back the first element that matches. .textContent is the element's text:",
      { code: page('<h1 id="title">Hello</h1>\n<p class="intro">Welcome to my page.</p>', 'const title = document.querySelector("#title");\nconsole.log(title.textContent);\nconsole.log(document.querySelector(".intro").textContent);') },
      "The selectors are the same as in CSS: #id, .class, tag names, and combinations.",
    ],
    [
      "When nothing matches, querySelector gives back null, and using null as if it were an element stops the script:",
      { code: page('<p id="message">Hi</p>', 'const missing = document.querySelector("#mesage");\nconsole.log(missing);\nconsole.log(missing.textContent);'), error: true },
      "#mesage is misspelled. In a browser the page still shows, but the script stops at that line, and the error appears only in the developer tools. Check there first when a script \"does nothing\".",
      { quiz: "What does querySelector give back when nothing matches?", choices: ["an empty element", "null"], answer: 2, why: "null: no element. Using it is a TypeError." },
    ],
    [
      {
        exercise: "scripts",
        task: 'The page has <h2 class="price">12 euros</h2> and <p id="stock">3 left</p>. Write a script that prints the price text, then the stock text.',
        expected: ["12 euros", "3 left"],
        starter: page('<h2 class="price">12 euros</h2>\n<p id="stock">3 left</p>', "// your code here"),
        hints: ['document.querySelector(".price").textContent', 'document.querySelector("#stock").textContent'],
        solution: page('<h2 class="price">12 euros</h2>\n<p id="stock">3 left</p>', 'console.log(document.querySelector(".price").textContent);\nconsole.log(document.querySelector("#stock").textContent);'),
      },
    ],
  ],
  recapPt: [
    "O JavaScript vai num <script>, normalmente no fim do body.",
    "document é a página; querySelector(seletor CSS) dá o primeiro elemento que encaixa.",
    ".textContent é o texto do elemento.",
    "Sem correspondência dá null, e usá-lo para o script; os erros aparecem no F12.",
  ],
  recap: [
    "JavaScript goes in a <script>, usually at the end of body.",
    "document is the page; querySelector(CSS selector) gives the first match.",
    ".textContent is the element's text.",
    "No match gives null, and using it stops the script; errors show in F12.",
  ],
};
