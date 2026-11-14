const { page } = require("./01-scripts.js");

module.exports = {
  id: "changing",
  title: "Changing the page",
  titlePt: "Mudar a página",
  screens: [
    [
      "Setting .textContent replaces an element's text, and the browser redraws it at once:",
      { code: page('<p id="greeting">Hello</p>', 'const p = document.querySelector("#greeting");\nconsole.log("before:", p.textContent);\np.textContent = "Hello, Ana!";\nconsole.log("after:", p.textContent);') },
    ],
    [
      "classList adds and removes classes, so CSS rules for those classes apply or stop applying. This is the usual way to change how something looks:",
      { code: page('<p id="box" class="card">Box</p>', 'const box = document.querySelector("#box");\nbox.classList.add("highlight");\nconsole.log(box.className);\nbox.classList.remove("card");\nconsole.log(box.className);\nconsole.log(box.classList.contains("highlight"));') },
      "classList.toggle(\"x\") adds x if it is missing and removes it if it is there.",
    ],
    [
      "querySelectorAll gives every match, and for...of goes through them:",
      { code: page("<ul>\n  <li>milk</li>\n  <li>bread</li>\n  <li>eggs</li>\n</ul>", 'const items = document.querySelectorAll("li");\nconsole.log(items.length, "items");\nfor (const item of items) {\n  item.textContent = item.textContent.toUpperCase();\n  console.log(item.textContent);\n}') },
      "Setting textContent puts plain text in, never HTML tags, which also makes it safe for text the visitor typed.",
      { quiz: "Which changes an element's look the usual way?", choices: ["classList.add, with a CSS rule for that class", "rewriting the whole page"], answer: 1, why: "Scripts switch classes; CSS decides what each class looks like." },
    ],
    [
      {
        exercise: "changing",
        task: 'The page has three li inside <ul id="tasks">. Give every li the class "todo", then set the first li\'s text to "DONE". Print each li\'s className and text, separated by a space.',
        expected: ["todo DONE", "todo Read", "todo Sleep"],
        starter: page('<ul id="tasks">\n  <li>Eat</li>\n  <li>Read</li>\n  <li>Sleep</li>\n</ul>', "// your code here"),
        hints: ['document.querySelectorAll("#tasks li") and a for...of with classList.add("todo")', 'document.querySelector("#tasks li").textContent = "DONE"; then a second loop that prints.'],
        solution: page('<ul id="tasks">\n  <li>Eat</li>\n  <li>Read</li>\n  <li>Sleep</li>\n</ul>', 'const items = document.querySelectorAll("#tasks li");\nfor (const item of items) {\n  item.classList.add("todo");\n}\nitems[0].textContent = "DONE";\nfor (const item of items) {\n  console.log(item.className, item.textContent);\n}'),
      },
    ],
  ],
  recapPt: [
    "Mudar .textContent muda o texto do elemento.",
    "classList.add / remove / toggle / contains trabalham com classes.",
    "querySelectorAll dá todas as correspondências; for...of percorre-as.",
  ],
  recap: [
    "Setting .textContent changes the element's text.",
    "classList.add / remove / toggle / contains work with classes.",
    "querySelectorAll gives every match; for...of goes through them.",
  ],
};
